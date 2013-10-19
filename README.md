DiscoSkirt
==========

An Arduino sketch for a skirt that had RGB LEDs and can respond to motion or sound.

The hardware consists of
* A [http://www.adafruit.com/products/659](Flora) wearable microcontroller.
* A Flora [http://www.adafruit.com/products/1247](Accelerometer Sensor) based on the LSM303
* A [https://www.sparkfun.com/products/9868](ADMP401) MEMS microphone breakout board
* 12 Flora [http://www.adafruit.com/products/1260](Smart Neo Pixels)
* A switch

Build instructions can be found at [http://www.gregington.com/2013/10/disco-skirt.html].


Libraries
---------
The sketch needs to have the following libraries installed in the Arduino IDE:
* [https://github.com/adafruit/Adafruit_Sensor](Adafruit_Sensor) - the Base Adafruit sensor library
* [https://github.com/adafruit/Adafruit_LSM303DLHC](Adafruit_LSM303) - to read the accelerometer
* [https://github.com/adafruit/Adafruit_NeoPixel](Adafruit_NeoPixel) - to drive the LEDs
* [https://code.google.com/p/arduino-pinchangeint/](PinChangeInt) - An interrupt library
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

