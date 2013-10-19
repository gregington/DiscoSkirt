/** Fade from black to colour, sparkle, then fade to black **/

void fadeAndSparkle() {
  uint8_t color[3];
  uint8_t hue = random(360);
  
  for (int val = 0; val < 256; val++) {
    getRGB(hue, 255, (uint8_t) val, color);
    for (int i = 0; i < numPixels; i++) {
      strip.setPixelColor(i, color[0], color[1], color[2]);
    }
    strip.show();
    DELAY(5);
  }

  DELAY(1000);
  
  for (int i = 0; i < 30; i++) {
    int numTwinkles = random(numPixels / 2);
    for (int j = 0; j < numTwinkles; j++) {
      if (random(2)   == 0) {
        strip.setPixelColor(random(numPixels), 0, 0, 0);
      } else {
        strip.setPixelColor(random(numPixels), 255, 255, 255);
      }
    }
    strip.show();
    DELAY(20);
    for (int i = 0; i < numPixels; i++) {
      strip.setPixelColor(i, color[0], color[1], color[2]);
    }
    strip.show();
  }

  for (int val = 255; val >= 0; val--) {
    getRGB(hue, 255, (uint8_t) val, color);
    for (int i = 0; i < numPixels; i++) {
      strip.setPixelColor(i, color[0], color[1], color[2]);
    }
    strip.show();
    DELAY(5);
  }
  
  DELAY(500);  
}

