/** Cycle through color wheel, LEDS equally spaced through hue **/

const int colorWheelAngle = 360 / numPixels;

void colorWheelCycle() {
  uint8_t color[3];
  for (int i = 0; i < 360; i++) { 
    for (int j = 0; j < numPixels; j++) {
      getRGB((i + (j * colorWheelAngle)) % 360, 255, 128, color);
      strip.setPixelColor(j, color[0], color[1], color[2]);
    }
    strip.show();
    DELAY(1);
  }  
}
