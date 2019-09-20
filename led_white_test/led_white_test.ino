#include <Adafruit_NeoPixel.h>

#define LED_PIN                   9 // NeoPixel strip is connected here
#define NUM_PIXELS               25

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  // put your setup code here, to run once:
  strip.begin();
  strip.show();
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < NUM_PIXELS; i++)
  {
    strip.setPixelColor(i, 255, 255, 255);
  }
  strip.show();
}
