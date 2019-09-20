#include <Adafruit_NeoPixel.h>

#define GEMMA
#ifdef GEMMA
#define STEP_PIN                  1 // Analog input for footstep
#define LED_PIN                   0 // NeoPixel strip is connected here
#endif

#ifndef GEMMA
#define STEP_PIN  A9
#define LED_PIN    6
#endif

#define NUM_PIXELS               10





Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  // put your setup code here, to run once:
  pinMode(2, INPUT_PULLUP); // Set internal pullup resistor for sensor pin
  strip.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  int input = analogRead(STEP_PIN);
  int red, green, blue;
  
  if (input < 100)
  {
    red = green = 100;
    blue = 0;
  }
  else if (input >= 100)
  {
    red = 100;
    green = 0;
    blue = 0;
  }  
  
  for (int i = NUM_PIXELS - 1; i >= 0 ; i--)
  {
    bool lightIt = (((input >> NUM_PIXELS - i) & 0x0001) == 0x0001);
    if (lightIt)
    {
      strip.setPixelColor(i, red, green, blue);
    }
    else
    {
      strip.setPixelColor(i, 0, 0, 0);
    }
  }

  strip.show();
  delay(1000);
}
