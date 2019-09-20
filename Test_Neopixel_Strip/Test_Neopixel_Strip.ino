#include <Adafruit_NeoPixel.h>

#define NUM_PIXELS  28
#define LED_PIN      9

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, LED_PIN); 

void setup() {
  // put your setup code here, to run once:
  strip.begin();
  strip.show();
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < NUM_PIXELS; i++)
  {
    strip.setPixelColor(i, 128, 64, 220);
    strip.show();
    delay(100);
    strip.setPixelColor(i, 0, 0, 0);
  }
}
