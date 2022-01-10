# phpi2c

The I2C PHP Extension provides an object-oriented wrapper to the
[i2c-dev](https://www.kernel.org/doc/Documentation/i2c/dev-interface) Linux Module that gives users the ability to write
code that interacts with external hardware using the [I2C](https://en.wikipedia.org/wiki/I%C2%B2C) protocol.

## Requirements

- PHP >= 8.0 (either NTS or ZTS version);
- libi2c-dev;

> **Note:** if you are using this extension on Raspbian or Raspberry Pi OS, the user executing the script **must**
> belong to the **i2c** group or you may run into "Permission denied" errors.

## Building

```bash
phpize
./configure
make
make test
```
## Stubs

After changing any of the [stubs](stubs/), the main [stub file](phpi2c.stub.php) must be updated by running the command below.

```bash
php stubs/update-main-stup.php
```

Once the stub is updated, the regular build process can be executed.

**Note:** The main stub file ([phpi2c.stub.php](phpi2c.stub.php)) is a generated file, edit the files on the [stubs](stubs/) folder instead.

## Classes

Class                                | Description
-------------------------------------|------------
[I2C\Bus](stubs/Bus.php)             | Represents a I2C Bus.
[I2C\Exception](stubs/Exception.php) | Base exception.

## Resources

- https://github.com/amaork/libi2c

## License

This library is licensed under the [PHP License](LICENSE).
