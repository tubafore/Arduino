#include <Adafruit_NeoPixel.h>

//#define FLORA
#define GEMMA

#ifdef FLORA
#define SENSOR_PIN                A9 // Analog input for flex sensor
#define LED_PIN                   8 // NeoPixel strip is connected here
#define NUM_PIXELS                1
#define ONBOARD_PIN               7 // the pin that the LED is attached to
#endif

#ifdef GEMMA
#define SENSOR_PIN                1 // Analog input for flex sensor
#define LED_PIN                   0 // NeoPixel strip is connected here
#define NUM_PIXELS                1
#define ONBOARD_PIN               1 // the pin that the LED is attached to
#endif


#define FADE_IN                   1
#define FADE_OUT                 -1


int brightness = 0;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by
int fadeState  = 0;
bool resting   = false;
int delayTime  = 30;


Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  pinMode(ONBOARD_PIN, OUTPUT);
  #ifdef FLORA
  Serial.begin(9600);
  #endif
  strip.begin();
  strip.setBrightness(255);
  strip.setPixelColor(0, 156, 0, 255);
  strip.show();
}

void loop() {
  int sensorInput;

  sensorInput = analogRead(SENSOR_PIN);
  #ifdef FLORA
    Serial.print(sensorInput);
  #endif
  sensorInput = map(sensorInput, 274, 130, 0, 90);
    
  #ifdef FLORA
    Serial.print(", ");
    Serial.println(sensorInput);
  #endif
  if (resting && sensorInput < 20)
    resting = false;
  if (resting || sensorInput < 20)
  {
    digitalWrite(ONBOARD_PIN, LOW);
    fadeState = 0;
    brightness = 0;
    strip.setBrightness(brightness);
  }
  else {
      if (fadeState == 0) {
        fadeState = FADE_IN;
        delayTime = 5;
      }
      digitalWrite(ONBOARD_PIN, HIGH);
      strip.setPixelColor(0, 80, 255, 0);
      brightness += fadeState * fadeAmount;
      strip.setBrightness(brightness);
      if (fadeState == FADE_IN && brightness >= 255) {
        fadeState = FADE_OUT;
        delayTime = 30;
      }
      else if (fadeState == FADE_OUT && brightness <= 0)
        resting = true;
  }
  
  strip.show();
  delay(delayTime);
}

