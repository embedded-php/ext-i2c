/*
   +----------------------------------------------------------------------+
   | Copyright (c) The PHP Group                                          |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Author: Flavio Heleno <https://github.com/flavioheleno>              |
   +----------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
  #include "config.h"
#endif

#include "bus.h"
#include "i2c.h"
#include "i2c_arginfo.h"
#include "zend_exceptions.h"
#include "zend_interfaces.h"
#include "zend_object_handlers.h"

#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>

// #define I2C_SLAVE 0x0703
// #define I2C_SMBUS 0x0720

// struct i2c_smbus_ioctl_data {
//   char read_write;
//   uint8_t command;
//   int size;
//   union i2c_smbus_data *data;
// };

/* zend_object wrapper to ensure i2c is handled properly */
typedef struct _busObject {
  int fd;
  zend_object zendObject;
} busObject;

static zend_object_handlers busObjectHandlers;

/********************************/
/* Extension's Internal Methods */
/********************************/

/* retrieve busObject pointer from a zend_object pointer */
static inline busObject *getBusObject(zend_object *obj) {
  return (busObject *)((char *)(obj) - XtOffsetOf(busObject, zendObject));
}

/* handle busObject release */
static void busFreeObject(zend_object *obj) {
  busObject *self = getBusObject(obj);

  /* failure to retrieve i2c instance (edge case?) */
  if (self == NULL) {
    return;
  }

  /* if the fd was open, close it */
  if (self->fd >= 0) {
    close(self->fd);
    self->fd = -1;
  }

  /* release self->zendObject */
  zend_object_std_dtor(&self->zendObject);
}

/* custom unset($inst->prop) handler */
static void unsetPropertyObjectHandler(zend_object *object, zend_string *offset, void **cache_slot) {
  zend_throw_error(NULL, "Cannot unset I2C\\Bus::$%s property", ZSTR_VAL(offset));
}

/********************************/
/* Extension's External Methods */
/********************************/

zend_class_entry* registerBusClass(void) {
  zend_class_entry ce, *classEntry;

  INIT_CLASS_ENTRY(ce, "I2C\\Bus", class_I2C_Bus_methods);
  classEntry = zend_register_internal_class(&ce);
  /* Final class / Objects of this class may not have dynamic properties */
  classEntry->ce_flags |= ZEND_ACC_FINAL | ZEND_ACC_NO_DYNAMIC_PROPERTIES;
  /* intercept object creation to change object handlers */
  classEntry->create_object = busCreateObject;

  /* disable serialization/unserialization */
  #if PHP_VERSION_ID >= 80102
    classEntry->ce_flags |= ZEND_ACC_NOT_SERIALIZABLE;
  #else
    classEntry->serialize = zend_class_serialize_deny;
    classEntry->unserialize = zend_class_unserialize_deny;
  #endif

  /* initialize busObjectHandlers with standard object handlers */
  memcpy(&busObjectHandlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

  /* disable object cloning */
  busObjectHandlers.clone_obj = NULL;
  /* Handler for objects that cannot be meaningfully compared. */
  /* busObjectHandlers.compare = zend_objects_not_comparable; */
  /* not yet sure what this sets */
  busObjectHandlers.offset   = XtOffsetOf(busObject, zendObject);
  /* hook the object release */
  busObjectHandlers.free_obj = busFreeObject;
  /* hook the object property unset */
  busObjectHandlers.unset_property = unsetPropertyObjectHandler;

  zval propDefaultValue;
  zend_string *propName;
  /* default property value (undefined) */
  ZVAL_UNDEF(&propDefaultValue);
  propName = zend_string_init("busId", sizeof("busId") - 1, false);
  /* private int $busId */
  zend_declare_typed_property(
    classEntry,
    propName,
    &propDefaultValue,
    ZEND_ACC_PRIVATE,
    NULL,
    (zend_type)ZEND_TYPE_INIT_MASK(MAY_BE_LONG)
  );
  zend_string_release(propName);

  propName = zend_string_init("deviceAddress", sizeof("deviceAddress") - 1, false);
  /* private int $deviceAddress */
  zend_declare_typed_property(
    classEntry,
    propName,
    &propDefaultValue,
    ZEND_ACC_PRIVATE,
    NULL,
    (zend_type)ZEND_TYPE_INIT_MASK(MAY_BE_LONG)
  );
  zend_string_release(propName);

  return classEntry;
}

zend_object *busCreateObject(zend_class_entry *zceClass) {
  busObject *self = zend_object_alloc(sizeof(busObject), zceClass);

  zend_object_std_init(&self->zendObject, zceClass);
  object_properties_init(&self->zendObject, zceClass);

  /* set object handlers */
  self->zendObject.handlers = &busObjectHandlers;

  return &self->zendObject;
}

/********************************/
/* PHP Visible Methods          */
/********************************/

/* {{{ I2C\Bus::__construct(int $busId, int $deviceAddress): void */
PHP_METHOD(I2C_Bus, __construct) {
  zend_long busId;
  zend_long deviceAddress;
  ZEND_PARSE_PARAMETERS_START(2, 2)
    Z_PARAM_LONG(busId)
    Z_PARAM_LONG(deviceAddress)
  ZEND_PARSE_PARAMETERS_END();

  busObject *self = getBusObject(Z_OBJ_P(ZEND_THIS));

  char device[20];
  if (snprintf(device, sizeof(device), "/dev/i2c-%ld", busId) >= 20) {
    zend_throw_exception_ex(zceException, 0, "The bus and/or the device number is invalid");

    RETURN_THROWS();
  }

  self->fd = open(device, O_RDWR);
  if (self->fd < 0) {
    zend_throw_exception_ex(zceException, 0, "Failed to open the i2c bus '%s'", device);

    RETURN_THROWS();
  }

  int err;
  errno = 0;
  err = ioctl(self->fd, I2C_SLAVE, deviceAddress);
  if (err < 0) {
    close(self->fd);
    zend_throw_exception_ex(zceException, errno, "Unable to select the i2c device: %s", strerror(errno));

    RETURN_THROWS();
  }

  /* update class property with constructor argument values */
  zend_update_property_long(zceBus, Z_OBJ_P(ZEND_THIS), "busId", sizeof("busId") - 1, busId);
  zend_update_property_long(zceBus, Z_OBJ_P(ZEND_THIS), "deviceAddress", sizeof("deviceAddress") - 1, deviceAddress);
}
/* }}} */

/* {{{ I2C\Bus::getBusId(): int */
PHP_METHOD(I2C_Bus, getBusId) {
  ZEND_PARSE_PARAMETERS_NONE();

  zval rv;
  zval *busId = zend_read_property(zceBus, Z_OBJ_P(ZEND_THIS), "busId", sizeof("busId") - 1, true, &rv);

  RETURN_LONG(zval_get_long(busId));
}
/* }}} */

/* {{{ I2C\Bus::getDeviceAddress(): int */
PHP_METHOD(I2C_Bus, getDeviceAddress) {
  ZEND_PARSE_PARAMETERS_NONE();

  zval rv;
  zval *deviceAddress = zend_read_property(zceBus, Z_OBJ_P(ZEND_THIS), "deviceAddress", sizeof("deviceAddress") - 1, true, &rv);

  RETURN_LONG(zval_get_long(deviceAddress));
}
/* }}} */

/* {{{ I2C\Bus::read(): int */
PHP_METHOD(I2C_Bus, read) {
  ZEND_PARSE_PARAMETERS_NONE();

  // struct i2c_smbus_ioctl_data i2cBuffer;
  // union i2c_smbus_data data;

  // memset(&i2cBuffer, 0, sizeof(i2cBuffer));
  // memset(&data, 0, sizeof(data));
  // i2cBuffer.read_write = I2C_SMBUS_READ;
  // i2cBuffer.command    = 0;
  // i2cBuffer.size       = I2C_SMBUS_BYTE;
  // i2cBuffer.data       = &data;

  busObject *self = getBusObject(Z_OBJ_P(ZEND_THIS));
  // if (ioctl(self->fd, I2C_SMBUS, &i2cBuffer)) {
  //   RETURN_LONG(-1);
  // }

  // RETURN_LONG(data.byte & 0xFF);

  int ret;
  errno = 0;
  ret = i2c_smbus_read_byte(self->fd);
  if (ret < 0) {
    zend_throw_exception_ex(zceException, errno, "Failed to read from the I2C device: %s", strerror(errno));

    RETURN_THROWS();
  }

  RETURN_LONG(ret & 0xFF);
}
/* }}} */

/* {{{ I2C\Bus::readByte(int $regAddress): int */
PHP_METHOD(I2C_Bus, readByte) {
  zend_long regAddress;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_LONG(regAddress)
  ZEND_PARSE_PARAMETERS_END();

  // struct i2c_smbus_ioctl_data i2cBuffer;
  // union i2c_smbus_data data;

  // memset(&i2cBuffer, 0, sizeof(i2cBuffer));
  // memset(&data, 0, sizeof(data));
  // i2cBuffer.read_write = I2C_SMBUS_READ;
  // i2cBuffer.command    = regAddress;
  // i2cBuffer.size       = I2C_SMBUS_BYTE_DATA;
  // i2cBuffer.data       = &data;

  busObject *self = getBusObject(Z_OBJ_P(ZEND_THIS));
  // if (ioctl(self->fd, I2C_SMBUS, &i2cBuffer)) {
  //   RETURN_LONG(-1);
  // }

  // RETURN_LONG(data.byte & 0xFF);

  int ret;
  errno = 0;
  ret = i2c_smbus_read_byte_data(self->fd, regAddress);
  if (ret < 0) {
    zend_throw_exception_ex(zceException, errno, "Failed to read a byte from the I2C device: %s", strerror(errno));

    RETURN_THROWS();
  }

  RETURN_LONG(ret & 0xFF);
}
/* }}} */

/* {{{ I2C\Bus::readWord(int $regAddress): int */
PHP_METHOD(I2C_Bus, readWord) {
  zend_long regAddress;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_LONG(regAddress)
  ZEND_PARSE_PARAMETERS_END();

  // struct i2c_smbus_ioctl_data i2cBuffer;
  // union i2c_smbus_data data;

  // memset(&i2cBuffer, 0, sizeof(i2cBuffer));
  // memset(&data, 0, sizeof(data));
  // i2cBuffer.read_write = I2C_SMBUS_READ;
  // i2cBuffer.command    = regAddress;
  // i2cBuffer.size       = I2C_SMBUS_WORD_DATA;
  // i2cBuffer.data       = &data;

  busObject *self = getBusObject(Z_OBJ_P(ZEND_THIS));
  // if (ioctl(self->fd, I2C_SMBUS, &i2cBuffer)) {
  //   RETURN_LONG(-1);
  // }

  // RETURN_LONG(data.word & 0xFFFF);

  int ret;
  errno = 0;
  ret = i2c_smbus_read_word_data(self->fd, regAddress);
  if (ret < 0) {
    zend_throw_exception_ex(zceException, errno, "Failed to read a word from the I2C device: %s", strerror(errno));

    RETURN_THROWS();
  }

  RETURN_LONG(ret & 0xFFFF);
}
/* }}} */

/* {{{ I2C\Bus::readBlock(int $regAddress, int $numBytes): array */
PHP_METHOD(I2C_Bus, readBlock) {
  zend_long regAddress;
  zend_long numBytes;
  ZEND_PARSE_PARAMETERS_START(2, 2)
    Z_PARAM_LONG(regAddress)
    Z_PARAM_LONG(numBytes)
  ZEND_PARSE_PARAMETERS_END();

  uint8_t *rxBuffer;
  rxBuffer = (uint8_t *)emalloc(sizeof(uint8_t) * numBytes);
  memset(rxBuffer, 0, sizeof(rxBuffer));

  busObject *self = getBusObject(Z_OBJ_P(ZEND_THIS));

  errno = 0;
  int ret = i2c_smbus_read_block_data(self->fd, regAddress, rxBuffer);
  if (ret < 1) {
    zend_throw_exception_ex(zceException, errno, "Failed to read a block from the I2C device: %s", strerror(errno));
    efree(rxBuffer);

    RETURN_THROWS();
  }

  zval zv;
  array_init_size(&zv, numBytes);
  for (int i = 0; i < numBytes; i++) {
    add_next_index_long(&zv, rxBuffer[i]);
  }

  efree(rxBuffer);

  RETURN_ARR(Z_ARRVAL(zv));
}
/* }}} */

/* {{{ I2C\Bus::write(int $data): void */
PHP_METHOD(I2C_Bus, write) {
  zend_long data;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_LONG(data)
  ZEND_PARSE_PARAMETERS_END();

  /*
  // struct i2c_smbus_ioctl_data i2cBuffer;

  // memset(&i2cBuffer, 0, sizeof(i2cBuffer));
  // i2cBuffer.read_write = I2C_SMBUS_WRITE;
  // i2cBuffer.command    = data;
  // i2cBuffer.size       = I2C_SMBUS_BYTE;
  // i2cBuffer.data       = NULL;
  */

  busObject *self = getBusObject(Z_OBJ_P(ZEND_THIS));
  // RETURN_LONG(ioctl(self->fd, I2C_SMBUS, &i2cBuffer));

  int err;
  errno = 0;
  err = i2c_smbus_write_byte(self->fd, data & 0xFF);
  if (err < 0) {
    zend_throw_exception_ex(zceException, errno, "Failed to send a word to the I2C device: %s", strerror(errno));

    RETURN_THROWS();
  }
}
/* }}} */

/* {{{ I2C\Bus::writeByte(int $regAddress, int $byte): void */
PHP_METHOD(I2C_Bus, writeByte) {
  zend_long regAddress;
  zend_long byte;
  ZEND_PARSE_PARAMETERS_START(2, 2)
    Z_PARAM_LONG(regAddress)
    Z_PARAM_LONG(byte)
  ZEND_PARSE_PARAMETERS_END();

  // struct i2c_smbus_ioctl_data i2cBuffer;
  // union i2c_smbus_data data;

  // memset(&i2cBuffer, 0, sizeof(i2cBuffer));
  // memset(&data, 0, sizeof(data));
  // i2cBuffer.read_write = I2C_SMBUS_WRITE;
  // i2cBuffer.command    = regAddress;
  // i2cBuffer.size       = I2C_SMBUS_BYTE_DATA;
  // i2cBuffer.data       = &data;

  // data.byte = byte & 0xFF;

  busObject *self = getBusObject(Z_OBJ_P(ZEND_THIS));
  // RETURN_LONG(ioctl(self->fd, I2C_SMBUS, &i2cBuffer));

  int err;
  errno = 0;
  err = i2c_smbus_write_byte_data(self->fd, regAddress, byte & 0xFF);
  if (err < 0) {
    zend_throw_exception_ex(zceException, errno, "Failed to send a byte to the I2C device: %s", strerror(errno));

    RETURN_THROWS();
  }
}
/* }}} */

/* {{{ I2C\Bus::writeWord(int $regAddress, int $word): void */
PHP_METHOD(I2C_Bus, writeWord) {
  zend_long regAddress;
  zend_long word;
  ZEND_PARSE_PARAMETERS_START(2, 2)
    Z_PARAM_LONG(regAddress)
    Z_PARAM_LONG(word)
  ZEND_PARSE_PARAMETERS_END();

  // struct i2c_smbus_ioctl_data i2cBuffer;
  // union i2c_smbus_data data;

  // memset(&i2cBuffer, 0, sizeof(i2cBuffer));
  // memset(&data, 0, sizeof(data));
  // i2cBuffer.read_write = I2C_SMBUS_WRITE;
  // i2cBuffer.command    = regAddress;
  // i2cBuffer.size       = I2C_SMBUS_WORD_DATA;
  // i2cBuffer.data       = &data;

  // data.word = word & 0xFFFF;

  busObject *self = getBusObject(Z_OBJ_P(ZEND_THIS));
  // RETURN_LONG(ioctl(self->fd, I2C_SMBUS, &i2cBuffer));

  int err;
  errno = 0;
  err = i2c_smbus_write_word_data(self->fd, regAddress, word & 0xFFFF);
  if (err < 0) {
    zend_throw_exception_ex(zceException, errno, "Failed to send a word to the I2C device: %s", strerror(errno));

    RETURN_THROWS();
  }
}
/* }}} */

/* {{{ I2C\Bus::writeBlock(int $regAddress, int ...$bytes): void */
PHP_METHOD(I2C_Bus, writeBlock) {
  zend_long regAddress;
  zval *args;
  int argc;
  ZEND_PARSE_PARAMETERS_START(2, -1)
    Z_PARAM_LONG(regAddress)
    Z_PARAM_VARIADIC('+', args, argc)
  ZEND_PARSE_PARAMETERS_END();

  if (argc > 32) {
    zend_throw_exception_ex(zceException, errno, "Cannot send more than 32 bytes in one go");

    RETURN_THROWS();
  }

  uint8_t *txBuffer;
  txBuffer = (uint8_t *)emalloc(sizeof(uint8_t) * argc);
  memset(txBuffer, 0, sizeof(txBuffer));

  for (int i = 0; i < argc; i++) {
    zval *arg = args + i;
    txBuffer[i] = zval_get_long(arg) & 0xFF;
  }

  busObject *self = getBusObject(Z_OBJ_P(ZEND_THIS));

  int err;
  errno = 0;
  err = i2c_smbus_write_block_data(self->fd, regAddress, argc, txBuffer);
  if (err < 0) {
    zend_throw_exception_ex(zceException, errno, "Failed to send a block to the I2C device: %s", strerror(errno));

    RETURN_THROWS();
  }
}
/* }}} */
