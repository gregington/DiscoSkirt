/** Sound activated visualisation using a Fast Hartley Transform (FHT) **/

const byte fhtColors[][3] = { {   0,   0,   0},
                         {   0, 200,   0},
                         {   0,   0, 200},
                         { 200,   0,   0},
                         { 200, 200,   0},
                         { 200,   0, 200},
                         {   0, 200, 200},
                         { 200, 200, 200} };


void soundVisualisation() {
  
  ADCSRA = 0xe5; // set the adc to free running mode
  ADCSRB = (ADCSRB & ~(1 << MUX5)) | (((13 >> 3) & 0x01) << MUX5);
  ADMUX = 0x40 | (13 & 0x7); // use adc13
  DIDR2 = 13; // turn off the digital input for adc13
  
  // Use out own while loop to reduce timing jutter
  while(1) {    
    TIMSK0 = 0; // turn off timer0 for lower jitter
    for (int i = 0 ; i < FHT_N ; i++) {
      while(!(ADCSRA & 0x10)); // wait for adc to be ready
      ADCSRA = 0xf5; // restart adc
      byte m = ADCL; // fetch adc data
      byte j = ADCH;
      int k = (j << 8) | m; // form into an int
      k -= 0x0200; // form into a signed int
      k <<= 6; // form into a 16b signed int
      fht_input[i] = k; // put real data into bins
    }
    TIMSK0 = 1; // turn on timer0 so we can use millis() again
    
    fht_window();
    fht_reorder();
    fht_run();
    fht_mag_octave();
    
    int index = 0;   
    int mx = fht_oct_out[0];
    for (int i = 1; i < 8; i++) {
      if (fht_oct_out[i] > mx) {
        index = i;
        mx = fht_oct_out[i];
      }
    }
    
    int r = fhtColors[index][0];
    int g = fhtColors[index][1];
    int b = fhtColors[index][2];
    
    for (int i = 0; i < numPixels; i++) {
      strip.setPixelColor(i, r, g, b);
    }
    
    strip.show();
    
    // Keep the color the same for 50 ms or the LEDs can flicker too quickly
    DELAY(50);
  }
}

