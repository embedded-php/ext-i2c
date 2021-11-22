--TEST--
Check if phpi2c is loaded
--SKIPIF--
<?php
if (! extension_loaded('phpi2c')) {
  exit('skip');
}
?>
--FILE--
<?php
echo 'The extension "phpi2c" is available';
?>
--EXPECT--
The extension "phpi2c" is available
