/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: e36176485b4c0bbfb1c1f412cfb8950b2f731809 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_I2C_Bus___construct, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, deviceId, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_I2C_Bus_read, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_I2C_Bus_readByte, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, register, IS_LONG, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_I2C_Bus_readWord arginfo_class_I2C_Bus_readByte

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_I2C_Bus_write, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_I2C_Bus_writeByte, 0, 2, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, register, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, value, IS_LONG, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_I2C_Bus_writeWord arginfo_class_I2C_Bus_writeByte


ZEND_METHOD(I2C_Bus, __construct);
ZEND_METHOD(I2C_Bus, read);
ZEND_METHOD(I2C_Bus, readByte);
ZEND_METHOD(I2C_Bus, readWord);
ZEND_METHOD(I2C_Bus, write);
ZEND_METHOD(I2C_Bus, writeByte);
ZEND_METHOD(I2C_Bus, writeWord);


static const zend_function_entry class_I2C_Bus_methods[] = {
	ZEND_ME(I2C_Bus, __construct, arginfo_class_I2C_Bus___construct, ZEND_ACC_PUBLIC)
	ZEND_ME(I2C_Bus, read, arginfo_class_I2C_Bus_read, ZEND_ACC_PUBLIC)
	ZEND_ME(I2C_Bus, readByte, arginfo_class_I2C_Bus_readByte, ZEND_ACC_PUBLIC)
	ZEND_ME(I2C_Bus, readWord, arginfo_class_I2C_Bus_readWord, ZEND_ACC_PUBLIC)
	ZEND_ME(I2C_Bus, write, arginfo_class_I2C_Bus_write, ZEND_ACC_PUBLIC)
	ZEND_ME(I2C_Bus, writeByte, arginfo_class_I2C_Bus_writeByte, ZEND_ACC_PUBLIC)
	ZEND_ME(I2C_Bus, writeWord, arginfo_class_I2C_Bus_writeWord, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};


static const zend_function_entry class_I2C_Exception_methods[] = {
	ZEND_FE_END
};
