/*
 * Ysera glove effect.  Bright flash and slow fade out using flex sensor.
 *
 * General Transistor Info - http://www.technologystudent.com/elec1/transis1.htm
 * Specific Transistor Info - https://en.wikipedia.org/wiki/2N2222
 * IR and Transistor Wiring Picture - https://learn.adafruit.com/using-an-infrared-library/sending-ir-codes
 * Flex Sensor Wiring Info - https://learn.adafruit.com/force-sensitive-resistor-fsr/using-an-fsr
 *
 * Circuit Construction
 * 
 * (Using Gemma)
 * 
 * 3Vo              - right pin of Flex sensor, when looking at the lettering
 * 10K Ohm Resistor - left pin of Flex sensor, when looking at the lettering
 * A1/D2            - left pin of Flex sensor, when looking at the lettering (this is not a typo)
 * 10K Ohm Resistor - GND
 * 3Vo              - +pin on NeoPixel
 * D0               - Input pin of NeoPixel (has -> after its connection point)
 * GND              - -pin on NeoPixel
 * D1               - 330 Ohm Resistor
 * 330 Ohm Resistor - P2N222A Transistor Base Pin (Middle Pin)
 * GND              - P2N222A Transistor Emitter Pin (Right Pin as you face the flat side)
 * -pin on IR LED   - P2N222A Transistor Collector Pin (Left Pin as you face the flat side)
 * 3Vo              - +pin on IR LED
 * 
 */

#include <Adafruit_NeoPixel.h>
#include <TinyIRremote.h>
#include <TinyIRremoteInt.h>

//#define FLORA
#define GEMMA

#ifdef FLORA
#define SENSOR_PIN                A9 // Analog input for flex sensor
#define LED_PIN                   8 // NeoPixel strip is connected here
#define NUM_PIXELS                7
#define ONBOARD_PIN               7 // the pin that the LED is attached to
#endif

#ifdef GEMMA
#define SENSOR_PIN                1 // Analog input for flex sensor
#define LED_PIN                   0 // NeoPixel strip is connected here
#define NUM_PIXELS                7
#define ONBOARD_PIN               1 // the pin that the LED is attached to
#endif


#define FADE_IN                   1
#define FADE_OUT                 -1
#define TRIGGER_ANGLE            40

int brightness = 0;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by
int fadeState  = 0;
bool resting   = false;
int delayTime  = 30;


Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);
IRsend sender;

void setup() {
  #ifdef FLORA
    pinMode(ONBOARD_PIN, OUTPUT);
    Serial.begin(9600);
  #endif
  strip.begin();
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
  if (resting && sensorInput < 10)
    resting = false;
  if (resting || sensorInput < TRIGGER_ANGLE)
  {
    #ifdef FLORA
      digitalWrite(ONBOARD_PIN, LOW);
    #endif
    fadeState = 0;
    brightness = 0;
  }
  else {
    if (fadeState == 0) {
      fadeState = FADE_IN;
      delayTime = 5;
      sender.sendNEC(0x20DF6897, 32);
      //return;
      //sender.sendNEC(0x20DF6897, 32);
      //sender.sendNEC(0x20DF6897, 32);
    }
    #ifdef FLORA
      digitalWrite(ONBOARD_PIN, HIGH);
    #endif
    for (int i = 0; i < NUM_PIXELS; i++)
    {
      strip.setPixelColor(i, 80, 255, 0);
    }
    brightness += fadeState * fadeAmount;

    if (fadeState == FADE_IN && brightness >= 255) {
      fadeState = FADE_OUT;
      delayTime = 30;
    }
    else if (fadeState == FADE_OUT && brightness <= 0)
      resting = true;
  }
  strip.setBrightness(brightness);
  strip.show();
  delay(delayTime);
}

