#include <IRLib.h>
#include <Adafruit_NeoPixel.h>

#define FLORA
#define PI 3.1415926535897932384626433832795

//#define GEMMA

/*#ifdef FLORA*/
#define LED_PIN                   9 // NeoPixel strip is connected here
#define NUM_PIXELS               47
#define ONBOARD_PIN               7 // the pin that the LED is attached to
#define IR_IN_PIN                 0 // the pin that the IR Sensor is attached to
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
uint32_t hitTime;
double pixelColors[NUM_PIXELS];
byte red = 80, green = 255;

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

  // Since sin is expensive to run, we calculate the fire effect once
  // and store the results
  for (int i = 0; i < NUM_PIXELS; i++)
  {
    pixelColors[i] = Evaluate(i);
  }
  
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
    DoFade(10);
    FireGlow();
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

    // If the decoder receives the code for '6', toggle the Hit Effect
    if (decoder.value == 0x20DF6897)
    {
      EnableHit();
    }

    // Once we're done processing the signal, clear the flag so we can process
    // the next one
    irSignalDetected = false;
  }
}

/*
 * The pulsing effect changes the brightness of the neopixels linearly between
 * 32 and 128 every "delayTime" milliseconds.
 */

void DoFade(int delayTime)
{
  if (fadeTime == 0 || millis() - fadeTime > delayTime)
  {
    fadeTime = millis();
    if (!rampUp)
      brightnessFactor--;
    else
      brightnessFactor++;
    if (brightnessFactor <= 32)
    {
      rampUp = true;
    }
    else if (brightnessFactor == 128)
    {
      rampUp = false;
    }

    //Brightness = 50% of 256 intervals
    brightness = brightnessFactor * 0.001953125f; // This is 1/512
  }
  else if (millis() < fadeTime)
  {
    fadeTime = millis();
  }
}

/*
 * Returns the result of the function sin(PI*x/NUM_PIXELS)
 * 
 * This function is symmetrical over the range 0 to NUM_PIXELS, so it gives
 * a consistent effect across the neopixel strip.
 *
 * The range of the output of this function is 0 to 1.
 */

double Evaluate(int x)
{
  return sin(((double)x*PI)/NUM_PIXELS);
}

/*
 * The Fire effect.
 *
 * This takes the result of sin function and scales the green channel by 255
 * its output.  This turns the color from red to orange to yellow.  To further
 * brighten the output, if the result is above .85, the output - 0.8 is similarly
 * scaled to the blue channel to effectively whiten the color.
 */

void FireGlow()
{
  double result;
  byte r, g, b;
  for (int i = 0; i < NUM_PIXELS; i++)
  {
    result = pixelColors[i];
    g = floor(255 * result * brightness);
    if (result > 0.85)
      b = floor (255 * (result - 0.8) * brightness);
    else
      b = 0;
    r = floor(255 * brightness);
    strip.setPixelColor(i, r, g, b);
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
  if (fadeTime - hitTime > 8000)
  {
    DisableHit();
  }
  if (brightnessFactor > 0)
  {
    if (brightnessFactor != 1)
      r = ceil(r * brightnessFactor *  0.00390625f);  // give it a little more red channel for a while
    else
      r = 0; // this ensures there is no dull red glow after the effect ends.
    g = floor(g * brightnessFactor *  0.00390625f);
    for (int i = 0; i < NUM_PIXELS; i++)
    {
      strip.setPixelColor(i, r, g, 0);  
    }
    brightnessFactor--;
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

