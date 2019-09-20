/*
 * Wiring Setup
 * 
 * For the Gemma
 * 
 * Vout  - Positive (Red) of Neopixel Strip
 * Gnd   - Ground (Black) of Neopixel Strip
 * D0    - Data line of Neopixel Strip
 * Al/D2 - One conductive thread 
 * Gnd   - The other conductive thread
 * D1    - Data line of the Constantly On Neopixels
 */

#include <Adafruit_NeoPixel.h>

#define FLORA
#undef FLORA

#ifdef FLORA
#define STEP_PIN  A9
#define LED_PIN    6
#define CONSTANT_LED_PIN         10 // NeoPixels that stay on are connected here
#endif

#ifndef FLORA
#define STEP_PIN                  1 // Analog input for footstep
#define LED_PIN                   0 // NeoPixel strip is connected here
#define CONSTANT_LED_PIN          1 // NeoPixels that stay on are connected here
#endif
#define NUM_PIXELS               28
#define NUM_CONSTANT_PIXELS       3
#define STEPPING_THRESHOLD      30
#define NOT_STEPPING_THRESHOLD  50
#define ALT_COLOR_COUNT           5
#define RESOLUTION                1
#define BLINK_DELAY            2000
#define BLINK_TIME               50
#define MAX_BRIGHTNESS          30.0
#define FLASH_BRIGHTNESS        128
#define STEPPING MAX_BRIGHTNESS / 256.0

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel constantStrip = Adafruit_NeoPixel(NUM_CONSTANT_PIXELS, CONSTANT_LED_PIN, NEO_GRB + NEO_KHZ800);

bool stepping;
bool wasStepping;
bool timeCounting;
bool blinkFlag;
int stepFiltered;
bool isSetup;
double Intensities [NUM_PIXELS];
int AltColorIndices[ALT_COLOR_COUNT];
long time;

void setup() {
  pinMode(2, INPUT_PULLUP); // Set internal pullup resistor for sensor pin
  stepping = false;
  strip.begin();
  constantStrip.begin();
  for (int i = 0; i < NUM_CONSTANT_PIXELS; i++)
  {
    constantStrip.setPixelColor(i, 0, 255, 0);
  }
  constantStrip.show();
  //  strip.setBrightness(255);
#ifdef FLORA
  Serial.begin(9600);
#endif
  stepFiltered = analogRead(STEP_PIN); // Initial input
}

void loop() {
  if (IsStepping())
  {
    if (!isSetup)
    {
      DoSetup();
    }
    FadeInColor();
    if (IsMaximumIntensity())
    {
      if (!timeCounting)
      {
        time = millis();
        timeCounting = true;
      }
      else
      {
        if (millis() - time >= BLINK_DELAY)
        {
          blinkFlag = true;
          time = millis();
        }
      }
    }
    RenderColors();
  }
  else if (wasStepping)
  {
    RenderColors();
    FadeOutColor();
  }
  else
  {
    ClearStrip();
    ClearSetup();
  }
  strip.show();
#ifdef FLORA
  Serial.print("Analog Reading is ");
  Serial.println(analogRead(STEP_PIN));
#endif
}

bool IsStepping() {
  stepFiltered = ((stepFiltered * 3) + analogRead(STEP_PIN)) >> 2;
  if (stepFiltered > NOT_STEPPING_THRESHOLD)
  {
    ClearSetup();
    if (stepping)
    {
      wasStepping = true;
    }
    stepping = false;
  }
  else if (stepFiltered <= STEPPING_THRESHOLD)
  {
    stepping = true;
    wasStepping = false;
  }

  return stepping;
}

void ClearStrip()
{
  for (int i = 0; i < NUM_PIXELS; i++)
  {
    strip.setPixelColor(i, 0, 0, 0);
  }
}

void RenderColors()
{
  //Serial.println("Setting colors from intensities");
  for (int i = 0; i < NUM_PIXELS; i++)
  {
    strip.setPixelColor(i, 0, floor(Intensities[i]), 0);
  }
  //Serial.println("Setting colors for alt color pixels");
  for (int i = 0; i < ALT_COLOR_COUNT; i++)
  {
    strip.setPixelColor(AltColorIndices[i]
                        , floor(0.615 * (Intensities[AltColorIndices[i]]))
                        , 0
                        , floor(Intensities[AltColorIndices[i]])
                       );
  }
  if (blinkFlag)
  {
    for (int i = 0; i < ALT_COLOR_COUNT; i++)
    {
      strip.setPixelColor(AltColorIndices[i], FLASH_BRIGHTNESS, FLASH_BRIGHTNESS, FLASH_BRIGHTNESS);
    }
    if (millis() - time >= BLINK_TIME)
    {
      blinkFlag = false;
      timeCounting = false;
      AssignAltColorIndices();
    }
  }
  //Serial.println("done rendering colors");
}

void DoSetup()
{
  for (int i = 0; i < NUM_PIXELS; i++)
  {
    Intensities[i] = 0;
  }
  AssignAltColorIndices();
  isSetup = true;
}

void AssignAltColorIndices()
{
  for (int i = 0; i < ALT_COLOR_COUNT; i++)
  {
    AltColorIndices[i] = random(NUM_PIXELS);
  }
}

void ClearSetup()
{
  isSetup = false;
  blinkFlag = false;
  timeCounting = false;
}

void FadeInColor()
{
  for (int i = 0; i < NUM_PIXELS; i++)
  {
    if (Intensities[i] <= (double)MAX_BRIGHTNESS - STEPPING)
      Intensities[i] += STEPPING;
  }
}

void FadeOutColor()
{
  bool doneFading = true;
  float fadeOutInterval = STEPPING * -2.0f;
  for (int i = 0; i < NUM_PIXELS; i++)
  {
    if (Intensities[i] >= 0)
    {
      Intensities[i] -= fadeOutInterval;
      doneFading = false;
    }
  }
  if (doneFading)
  {
    wasStepping = false;
    ClearStrip();
  }
}

bool IsMaximumIntensity()
{
  return (abs(Intensities[0] - MAX_BRIGHTNESS) < STEPPING);
}
