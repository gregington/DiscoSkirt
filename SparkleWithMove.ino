/** Sparkle when accelerometer detects movement */

// lower number = more sensitive
const float moveThreshold = 1.7;
  
void sparkleWithMove() {
  double magnitude = getMagnitude();
  // wait a bit
  DELAY(100);
  
  // get new data!
  double newMagnitude = getMagnitude();
  
  // are we moving 
  if (abs(magnitude - newMagnitude) > moveThreshold) {
    flashRandom(5, 1);  // first number is 'wait' delay, shorter num == shorter twinkle
    flashRandom(5, 3);  // second number is how many neopixels to simultaneously light up
    flashRandom(5, 2);
  }
}

float getMagnitude() {
  // Take a reading of accelerometer data
  sensors_event_t event; 
  accel.getEvent(&event);
 
  // Get the magnitude (length) of the 3 axis vector
  // http://en.wikipedia.org/wiki/Euclidean_vector#Length
  double storedVector = event.acceleration.x*event.acceleration.x;
  storedVector += event.acceleration.y*event.acceleration.y;
  storedVector += event.acceleration.z*event.acceleration.z;
  storedVector = sqrt(storedVector);
  return storedVector;
}

void flashRandom(int wait, uint8_t howmany) {
 
  for(uint16_t i=0; i<howmany; i++) {
    // pick a random color!
    byte color[3];
    getRGB(random(360), 255, 200, color);
 
    // get a random pixel from the list
    int j = random(strip.numPixels());
    
    // now we will 'fade' it in 5 steps
    for (int x=0; x < 5; x++) {
      byte r = color[0] * (x+1); r /= 5;
      byte g = color[1] * (x+1); g /= 5;
      byte b = color[2] * (x+1); b /= 5;
      
      strip.setPixelColor(j, strip.Color(r, g, b));
      strip.show();
      DELAY(wait);
    }
    // & fade out in 5 steps
    for (int x=5; x >= 0; x--) {
      byte r = color[0] * x; r /= 5;
      byte g = color[1] * x; g /= 5;
      byte b = color[2] * x; b /= 5;
      
      strip.setPixelColor(j, strip.Color(r, g, b));
      strip.show();
      DELAY(wait);
    }
  }
  // LEDs will be off when done (they are faded to 0)
}

