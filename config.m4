dnl config.m4 for extension i2c

PHP_ARG_ENABLE(
  i2c,
  whether to enable i2c support,
  [
    AS_HELP_STRING(
      [ --enable-i2c ],
      [ Include i2c support ]
    )
  ],
  no
)

if test "${PHP_I2C}" != "no"; then
  PHP_VERSION=$($PHP_CONFIG --vernum)
  AC_MSG_CHECKING([PHP version]);
  if test $PHP_VERSION -lt 80000; then
    AC_MSG_ERROR([i2c requires PHP 8.0+]);
  fi
  AC_MSG_RESULT([$PHP_VERSION])

  PHP_CHECK_LIBRARY(i2c, i2c_smbus_read_byte,
    [
      AC_DEFINE(HAVE_I2C, 1, [ Have i2c support ])
    ],
    [
      AC_MSG_ERROR([ libi2c not found ])
    ]
  )

  PHP_NEW_EXTENSION(i2c, i2c.c src/bus.c src/exception.c, $ext_shared)
fi
