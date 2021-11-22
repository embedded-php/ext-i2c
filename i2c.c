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
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "i2c.h"
#include "i2c_arginfo.h"

#include "src/bus.h"
#include "src/exception.h"

/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE() \
  ZEND_PARSE_PARAMETERS_START(0, 0) \
  ZEND_PARSE_PARAMETERS_END()
#endif

/* Class entry pointers (global resources) */
zend_class_entry *zceBus;
zend_class_entry *zceException;

/* {{{ PHP_RINIT_FUNCTION */
PHP_RINIT_FUNCTION(i2c) {
  #if defined(ZTS) && defined(COMPILE_DL_I2C)
    ZEND_TSRMLS_CACHE_UPDATE();
  #endif

  return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(i2c) {
  /* Extension constants */
  REGISTER_STRING_CONSTANT("I2C\\EXTENSION_VERSION", PHP_I2C_VERSION, CONST_CS | CONST_PERSISTENT);

  /* Class Registration (from each *.h file) */
  zceBus = registerBusClass();
  zceException = registerExceptionClass();

  return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION */
PHP_MINFO_FUNCTION(i2c) {
  php_info_print_table_start();
  php_info_print_table_header(2, "i2c support", "enabled");
  php_info_print_table_row(2, "Extension version", PHP_I2C_VERSION);
  php_info_print_table_end();
}
/* }}} */

/* {{{ i2c_module_entry */
zend_module_entry i2c_module_entry = {
  STANDARD_MODULE_HEADER,
  PHP_I2C_EXTNAME, /* Extension name */
  NULL,            /* zend_function_entry */
  PHP_MINIT(i2c),  /* PHP_MINIT - Module initialization */
  NULL,            /* PHP_MSHUTDOWN - Module shutdown */
  PHP_RINIT(i2c),  /* PHP_RINIT - Request initialization */
  NULL,            /* PHP_RSHUTDOWN - Request shutdown */
  PHP_MINFO(i2c),  /* PHP_MINFO - Module info */
  PHP_I2C_VERSION, /* Version */
  STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_I2C
  #ifdef ZTS
    ZEND_TSRMLS_CACHE_DEFINE()
  #endif
  ZEND_GET_MODULE(i2c)
#endif
