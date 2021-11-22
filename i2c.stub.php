<?php

/** @generate-function-entries */

namespace I2C;

final class Bus {
  /**
   * Opens the I2C Bus.
   *
   * @param string $path     Path to the I2C bus
   * @param int    $deviceId I2C device id
   *
   * @return void
   */
  public function __construct(string $path, int $deviceId) {}

  /**
   * Read from I2C bus.
   *
   * @return int
   */
  public function read(): int {}

  /**
   * Read a byte from a register.
   *
   * @param int $register Register address
   *
   * @return int
   */
  public function readByte(int $register): int {}

  /**
   * Read a word from a register.
   *
   * @param int $register Register address
   *
   * @return int
   */
  public function readWord(int $register): int {}

  /**
   * Write to I2C bus.
   *
   * @param int $data Data to be written
   *
   * @return int
   */
  public function write(int $data): int {}

  /**
   * Write a byte to a register.
   *
   * @param int $register Register address
   * @param int $value    Byte value
   *
   * @return int
   */
  public function writeByte(int $register, int $value): int {}

  /**
   * Write a word to a register.
   *
   * @param int $register Register address
   * @param int $value    Word value
   *
   * @return int
   */
  public function writeWord(int $register, int $value): int {}
}

/**
 * Base exception
 */
class Exception extends \Exception {}

