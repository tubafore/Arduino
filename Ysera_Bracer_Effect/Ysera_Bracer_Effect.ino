/*
 * Ysera Bracer Effect
 * 
 * Steady green  light, blinks orange and fades out when hit
 * 
 * Rx    - IR Sensor Left Pin as you face the round side
 * GND   - IR Sensor Middle Pin
 * VBatt - IR Sensor Right Pin 
 * VBatt - Jewel 5V DC Wire
 * GND   - Jewel GND Wire
 * D9    - Jewel Data Input Wire
 */

#include <IRLib.h>
#include <Adafruit_NeoPixel.h>

#define FLORA
#define PI 3.1415926535897932384626433832795

//#define GEMMA

/*#ifdef FLORA*/
#define LED_PIN                   9 // NeoPixel strip is connected here
#define NUM_PIXELS                7
#define ONBOARD_PIN               7 // the pin that the LED is attached to
#define IR_IN_PIN                 0 // the pin that the IR Sensor is attached to, this is RX
/*#endif */

//#ifdef GEMMA
//#define SENSOR_PIN                1 // Analog input for flex sensor
//#define LED_PIN                   0 // NeoPixel strip is connected here
//#define NUM_PIXELS                1
//#define ONBOARD_PIN               1 // the pin that the LED is attached to
//#endif

#define DEBUG
//#undef DEBUG

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, LED_PIN);
IRrecv receiver(IR_IN_PIN);
IRdecodeNEC decoder;

float brightness = 0.125f;
int brightnessFactor = 32;
bool rampUp = true;
uint32_t fadeTime = 0;
bool hitBySpell = false;
uint32_t hitTime, currentDifference;
byte red = 220, green = 80;
bool isOrange = false;
byte blinkSkip;
/*
 * this variable gets updated in the interrupt.  Marking it volatile
 * makes it such that the compiler doesn't make temporary copies in
 * CPU registers for speed purposes.  This is necessary to make sure
 * the loop gets the correct value.
 */ 

volatile bool irSignalDetected = false;

void setup() {
  #ifdef DEBUG
    Serial.begin(9600);
  #endif
  pinMode(0, INPUT_PULLUP);

  // Start up the neopixels and start them up all off
  strip.begin();
  strip.show();

  // Start up the IR Receiver 
  receiver.enableIRIn();
  
  // Sets up the interrupt function, IRActive, on interrupt pin 2 INT2
  // which is pin 0, RX on the flora.  This will be called every time
  // the signal on the pin goes HIGH.
  attachInterrupt(2, IRActive, RISING);
}

void loop() {
  if (irSignalDetected)
  {
    HandleIRSignal();
  }
  if (!hitBySpell)
  {
    for (int i = 0; i < NUM_PIXELS; i++)
    {
      strip.setPixelColor(i, 80, 255, 0);
    }
  }
  else
  {
    SpellHit();
  }
    
  strip.show();
  #ifdef DEBUG
    DebugOutput();
  #endif
}

/*
 * Interrupt code.
 * 
 * Set a flag to try to process IR only when we receive IR data.  If interrupts
 * weren't used, trying to decode IR signals would keep any additional processing
 * until successfully decoding.
 */
 
void IRActive() 
{
  irSignalDetected = true;
}

void HandleIRSignal()
{
  if (!hitBySpell) 
  {
    // GetResults returns true after it it finishes receiving a code
    while (!receiver.GetResults(&decoder))
    {

    }
    // The receiver needs to start trying again once it's gotten results 
    receiver.resume();

    // I'm not sure this is required, but decodes the message
    decoder.decode();

    // If the decoder receives the code for '4', toggle the Hit Effect
    if (decoder.value == 0x20DF28D7)
    {
      EnableHit();
    }

    // Once we're done processing the signal, clear the flag so we can process
    // the next one
    irSignalDetected = false;
  }
}

/*
 * The Spell Hit Effect
 * 
 * This starts at a bright green and fades out to black, not resuming the
 * normal fire effect until 8 seconds have passed.
 */

void SpellHit()
{
  if (millis() - fadeTime < 10)
    return;
  byte r = red, g = green; 
  fadeTime = millis();
  if (fadeTime - hitTime < 300)
  {
    if (blinkSkip++ < 5)
      return;
    for (int i = 0; i < NUM_PIXELS; i++)
    {
      if (!isOrange)
      {
        strip.setPixelColor(i, 80, 255, 0);        
      }
      else
      {
        strip.setPixelColor(i, 220, 80, 0);
      }
    }
    if (isOrange)
      isOrange = false;
    else
      isOrange = true;
    blinkSkip = 0;
  }
  else if (fadeTime - hitTime < 1500)
  {
    if (brightnessFactor > 0)
    {
      if (brightnessFactor > 5)
      {
        r = ceil(r * brightnessFactor *  0.00390625f);  // give it a little more red channel for a while
        g = floor(g * brightnessFactor *  0.00390625f);
      }
      else
      {
        r = 0; // this ensures there is no dull red glow after the effect ends.
        g = 0;
      }
      
      for (int i = 0; i < NUM_PIXELS; i++)
      {
        strip.setPixelColor(i, r, g, 0);  
      }
      brightnessFactor -= 5;
    }
  }
  else 
  {
    DisableHit();
  }
}

/*
 * Toggle on everything field that needs to change to do the spell hit effect
 */

void EnableHit()
{
  hitBySpell = true;
  hitTime = millis();
  brightnessFactor = 255;
  blinkSkip = 0;
  for (int i = 0; i < NUM_PIXELS; i++)
    strip.setPixelColor(i, 0, 0, 0);
}

/*
 * Toggle off everything field that needs to change to do the spell hit effect
 */

void DisableHit()
{
  hitBySpell = false;
  rampUp = true;
  brightness = 0;
  for (int i = 0; i < NUM_PIXELS; i++)
    strip.setPixelColor(i, 0, 0, 0);
}

/*
 * The debugging output so we can monitor the state of the program
 */

#ifdef DEBUG
void DebugOutput()
{
  Serial.print("Flags:  ");
  Serial.print(" hitBySpell ");
  Serial.print(hitBySpell ? "true" : "false");
  Serial.print(" irSignalDetected ");
  Serial.print(irSignalDetected ? "true" : "false");
  Serial.print(" hitTime ");
  Serial.print(hitTime, DEC);
  Serial.print(" fadeTime ");
  Serial.print(fadeTime, DEC);
  Serial.print(" brightness ");
  Serial.print(brightness, DEC);
  Serial.print(" brightnessFactor ");
  Serial.println(brightnessFactor);
}
#endif

