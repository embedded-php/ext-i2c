<?php

namespace I2C;

final class Bus {
  /**
   * Opens the I2C Bus.
   *
   * @param int $busId         I2C bus id
   * @param int $deviceAddress I2C device address
   *
   * @return void
   */
  public function __construct(int $busId, int $deviceAddress) {}

  /**
   * Return the I2C bus id.
   *
   * @return int
   */
  public function getBusId(): int {}

  /**
   * Return the Device Address.
   *
   * @return int
   */
  public function getDeviceAddress(): int {}

  /**
   * Read from I2C bus.
   *
   * @return int
   */
  public function read(): int {}

  /**
   * Read a byte from a register.
   *
   * @param int $regAddress Register address
   *
   * @return int
   */
  public function readByte(int $regAddress): int {}

  /**
   * Read a word from a register.
   *
   * @param int $regAddress Register address
   *
   * @return int
   */
  public function readWord(int $regAddress): int {}

  /**
   * Read a block of bytes up to $numBytes long from a register.
   * Note: I2C specification limits block reads to 32 bytes in a single read.
   *
   * @param int $regAddress Register address
   * @param int $numBytes   Number of bytes to read from device
   *
   * @return int[]
   */
  public function readBlock(int $regAddress, int $numBytes): array {}

  /**
   * Write to I2C bus.
   *
   * @param int $data Data to be written
   *
   * @return boolean
   */
  public function write(int $data): bool {}

  /**
   * Write a byte on a register.
   *
   * @param int $regAddress Register address
   * @param int $byte       Byte value
   *
   * @return boolean
   */
  public function writeByte(int $regAddress, int $byte): bool {}

  /**
   * Write a word on a register.
   *
   * @param int $regAddress Register address
   * @param int $word       Word value
   *
   * @return boolean
   */
  public function writeWord(int $regAddress, int $word): bool {}

  /**
   * Write a block of bytes on a register.
   * Note: I2C specification limits block writes to 32 bytes in a single write.
   *
   * @param int   $regAddress Register address
   * @param int[] $bytes      Bytes to be sent
   *
   * @return boolean
   */
  public function writeBlock(int $regAddress, int ...$bytes): bool {}
}
