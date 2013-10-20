// For PinChangeInt - No interrupts on ports C or D
#define NO_PORTC_PINCHANGES
#define NO_PORTD_PINCHANGES

// FHT configuration for sound visualisation
#define OCTAVE 1   // Group buckets into octaves
#define OCT_NORM 0 // Don't normalise octave intensities by number of bins
#define FHT_N 256


#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303.h>
#include <Adafruit_NeoPixel.h>
#include <PinChangeInt.h>
#include <FHT.h>

// Delay macro that checks for button presses signalled by interrupt
#define DELAY(x) if (delayAndCheckForButonPress(x) > 0) return;

const byte numPixels = 12;
const byte buttonPin = 9;

// RGB LEDs

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numPixels, 6, NEO_GRB + NEO_KHZ800);

// Accelerometer
Adafruit_LSM303_Accel accel = Adafruit_LSM303_Accel(54321);

// Function points to skirt modes. Comment out modes that are not required
void(*modes[]) () = { &sparkleWithMove,
                      &colorWheelCycle,
                      &fadeAndSparkle,
                      &chase,
                      &soundVisualisation };
                         
const byte numModes = sizeof(modes) / sizeof(modes[0]);

// Set by interrupt handler if it is time to change mode
volatile byte changeMode = 0;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  PCintPort::attachInterrupt(buttonPin, &pressHandler, FALLING);

  accel.begin();

  strip.begin();
  strip.show();
}

void loop() {
  static int mode = 0;  
  if (changeMode != 0) {
    mode = ((mode + 1) % numModes);
    resetToBlack();
    changeMode = 0;
  }
  (*modes[mode])(); // call the new mode function
}

void pressHandler() {
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();
  // debounce
  if (interruptTime - lastInterruptTime > 200) {
    changeMode = 1;
    lastInterruptTime = interruptTime;
  }
}

// Utilities

// Reset all pixels to black
void resetToBlack() {
  for (int i = 0; i < numPixels; i++) {
    strip.setPixelColor(i, 0, 0, 0);
  }
  strip.show();
}

// Delay for a specified duration by breaking down into a smaller number
// of delays and checking for a button press, returning early if
// there was a button press
int delayAndCheckForButonPress(long duration) {
  long target = millis() + duration;
  while(1) {
    long current = millis();
    if (current > target) {
      return changeMode;
    }
    long difference = target - current;
    if (difference < 10) {
      delay(difference);
    } else {
      delay(10);
    }
  }
}

// Gamma values used to convert HSV to RGB
PROGMEM const byte dim_curve[] = {
    0,   1,   1,   2,   2,   2,   2,   2,   2,   3,   3,   3,   3,   3,   3,   3,
    3,   3,   3,   3,   3,   3,   3,   4,   4,   4,   4,   4,   4,   4,   4,   4,
    4,   4,   4,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   6,   6,   6,
    6,   6,   6,   6,   6,   7,   7,   7,   7,   7,   7,   7,   8,   8,   8,   8,
    8,   8,   9,   9,   9,   9,   9,   9,   10,  10,  10,  10,  10,  11,  11,  11,
    11,  11,  12,  12,  12,  12,  12,  13,  13,  13,  13,  14,  14,  14,  14,  15,
    15,  15,  16,  16,  16,  16,  17,  17,  17,  18,  18,  18,  19,  19,  19,  20,
    20,  20,  21,  21,  22,  22,  22,  23,  23,  24,  24,  25,  25,  25,  26,  26,
    27,  27,  28,  28,  29,  29,  30,  30,  31,  32,  32,  33,  33,  34,  35,  35,
    36,  36,  37,  38,  38,  39,  40,  40,  41,  42,  43,  43,  44,  45,  46,  47,
    48,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,
    63,  64,  65,  66,  68,  69,  70,  71,  73,  74,  75,  76,  78,  79,  81,  82,
    83,  85,  86,  88,  90,  91,  93,  94,  96,  98,  99,  101, 103, 105, 107, 109,
    110, 112, 114, 116, 118, 121, 123, 125, 127, 129, 132, 134, 136, 139, 141, 144,
    146, 149, 151, 154, 157, 159, 162, 165, 168, 171, 174, 177, 180, 183, 186, 190,
    193, 196, 200, 203, 207, 211, 214, 218, 222, 226, 230, 234, 238, 242, 248, 255,
};

/*
 * Code taken from http://www.kasperkamperman.com/blog/arduino/arduino-programming-hsb-to-rgb/
 * hue ranges from 0 to 360
 * sat is from 0 to 255
 * val is from 0 to 255
 */
void getRGB(int hue, uint8_t sat, uint8_t val, uint8_t colors[3]) { 
  /* convert hue, saturation and brightness ( HSB/HSV ) to RGB
     The dim_curve is used only on brightness/value and on saturation (inverted).
     This looks the most natural.      
  */

  val = pgm_read_byte(&dim_curve[val]);
  sat = 255 - pgm_read_byte(dim_curve[255-sat]);

  int r;
  int g;
  int b;
  int base;

  if (sat == 0) { // Acromatic color (gray). Hue doesn't mind.
    colors[0]=val;
    colors[1]=val;
    colors[2]=val;  
  } else  { 

    base = ((255 - sat) * val)>>8;

    switch(hue/60) {
    case 0:
        r = val;
        g = (((val-base)*hue)/60)+base;
        b = base;
    break;

    case 1:
        r = (((val-base)*(60-(hue%60)))/60)+base;
        g = val;
        b = base;
    break;

    case 2:
        r = base;
        g = val;
        b = (((val-base)*(hue%60))/60)+base;
    break;

    case 3:
        r = base;
        g = (((val-base)*(60-(hue%60)))/60)+base;
        b = val;
    break;

    case 4:
        r = (((val-base)*(hue%60))/60)+base;
        g = base;
        b = val;
    break;

    case 5:
        r = val;
        g = base;
        b = (((val-base)*(60-(hue%60)))/60)+base;
    break;
    }

    colors[0]=(uint8_t) r;
    colors[1]=(uint8_t) g;
    colors[2]=(uint8_t) b; 
  }  
}

