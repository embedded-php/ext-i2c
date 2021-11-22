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
  [yes]
  [yes]
)

if test "$PHP_I2C" != "no"; then
  AC_DEFINE(HAVE_I2C, 1, [ Have i2c support ])

  PHP_NEW_EXTENSION(i2c, i2c.c src/bus.c src/exception.c, $ext_shared)
fi
