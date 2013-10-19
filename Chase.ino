/** Lights up all LEDs in sequence **/

void chase() {
  byte color[3];
  getRGB(random(360), 255, 200, color);
  for (int i = 0; i < numPixels; i++) {
    for (int j = 0; j < numPixels; j++) {
      if (i == j) {
        strip.setPixelColor(j, color[0], color[1], color[2]);
      } else {
        strip.setPixelColor(j, 0, 0, 0);
      }
    }
    strip.show();
    DELAY(100);
  }
}

