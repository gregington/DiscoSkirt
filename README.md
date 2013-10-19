DiscoSkirt
==========

An Arduino sketch for a skirt that had RGB LEDs and can respond to motion or sound.

The hardware consists of
* A [Flora](http://www.adafruit.com/products/659) wearable microcontroller.
* A Flora [Accelerometer Sensor](http://www.adafruit.com/products/1247) based on the LSM303
* A [ADMP401](https://www.sparkfun.com/products/9868) MEMS microphone breakout board
* 12 Flora [Smart Neo Pixels](http://www.adafruit.com/products/1260)
* A switch

Build instructions can be found at [http://www.gregington.com/2013/10/disco-skirt.html].


Libraries
---------
The sketch needs to have the following libraries installed in the Arduino IDE:
* [Adafruit_Sensor](https://github.com/adafruit/Adafruit_Sensor) - the Base Adafruit sensor library
* [Adafruit_LSM303](https://github.com/adafruit/Adafruit_LSM303DLHC) - to read the accelerometer
* [Adafruit_NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel) - to drive the LEDs
* [PinChangeInt](https://code.google.com/p/arduino-pinchangeint/) - An interrupt library
* [FHT](http://wiki.openmusiclabs.com/wiki/ArduinoFHT) - a Fast Hartley Transform for responding to sound


Code
----

The code is designed to make it easy to add and remove modes. Each mode is stored in a separate file.
To change modify the `modes` array to add or remove the required mode. Each entry in the array mist
be a function pointer to a function that takes no arguments. The specified function will be executed
continually in an infinite loop.

As an interrupt is used to trigger mode changes, use for the `delay()` function is discouraged. A
DELAY macro is available as a convenience that breaks larger delays into smaller ones, checking
for a mode change every 10ms. If a mode change has been detected, then control will be returned
to the main loop which will advance the mode.

