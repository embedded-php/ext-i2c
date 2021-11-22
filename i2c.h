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

#ifndef PHP_I2C_H
#define PHP_I2C_H

  extern zend_module_entry i2c_module_entry;
  #define phpext_i2c_ptr &i2c_module_entry

  #define PHP_I2C_EXTNAME   "phpi2c"
  #define PHP_I2C_VERSION   "0.0.1-dev"
  #define PHP_I2C_AUTHOR    "flavioheleno"
  #define PHP_I2C_URL       "https://github.com/flavioheleno/phpi2c"
  #define PHP_I2C_COPYRIGHT "Copyright (c) 2021"

  #if defined(ZTS) && defined(COMPILE_DL_I2C)
    ZEND_TSRMLS_CACHE_EXTERN()
  #endif

  BEGIN_EXTERN_C()

  /* Class entry pointers */
  extern zend_class_entry *zceBus;
  extern zend_class_entry *zceException;

  END_EXTERN_C()

#endif
