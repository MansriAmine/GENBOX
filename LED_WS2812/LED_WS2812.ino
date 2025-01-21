#include <FastLED.h>

// Define the number of LEDs and the data pin
#define NUM_LEDS 26
#define DATA_PIN 6

// Create an array of LEDs
CRGB leds[NUM_LEDS];

// Array of vibrant colors
const CRGB vibrantColors[] = {
  CRGB::Red,          // Red
  CRGB::Green,        // Green
  CRGB::Blue,         // Blue
  CRGB::Yellow,       // Yellow
  CRGB::Magenta,      // Magenta
  CRGB::Cyan,         // Cyan
  CRGB::Orange,       // Orange
  CRGB::Pink,         // Pink
  CRGB::Purple,       // Purple
  CRGB::Teal,         // Teal
  CRGB::White,        // White
  CRGB(255, 105, 180), // HotPink
  CRGB(0, 255, 255),  // Aqua
  CRGB(255, 69, 0),   // RedOrange
  CRGB(0, 255, 0),    // Lime
  CRGB(255, 20, 147)  // DeepPink
};

// Global brightness variable
uint8_t brightness = 50; // Set to 50% brightness by default

// Timing variables
unsigned long previousMillis = 0;
const long interval = 30000; // Interval to switch patterns (30 seconds)

int currentPattern = 0;

void setup() {
  // Initialize the FastLED library
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(brightness); // Set global brightness
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    currentPattern = (currentPattern + 1) % 2; // Cycle through 2 patterns
  }

  // Run the current pattern
  switch (currentPattern) {
    case 0:
      theaterChase();
      break;
    case 1:
      rainbow();
      break;
  }
}

// Pattern 1: Theater Chase
void theaterChase() {
  for (int j = 0; j < 10; j++) {
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < NUM_LEDS; i += 3) {
        leds[i + q] = vibrantColors[j % (sizeof(vibrantColors) / sizeof(vibrantColors[0]))];
      }
      FastLED.show();
      delay(100);
      for (int i = 0; i < NUM_LEDS; i += 3) {
        leds[i + q] = CRGB::Black;
      }
    }
  }
}

// Pattern 2: Rainbow
void rainbow() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(((i * 256 / NUM_LEDS) + millis() / 10) % 255, 255, 255);
  }
  FastLED.show();
  delay(20); // Delay to control the speed of the rainbow effect
}
