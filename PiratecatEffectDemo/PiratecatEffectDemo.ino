#include <Adafruit_NeoPixel.h>

#include "PiratecatEffect.h"
#include "Wrath.h"
#include "Lifebloom.h"

#define HAIR_LED_PIN                  10 // NeoPixel strip is connected here
#define HAIR_NUM_PIXELS               12
#define GLOVE_LED_PIN                 12
#define GLOVE_NUM_PIXELS               7
#define SENSOR_PIN                9 // Analog input for flex sensor

#define DEBUG
#undef DEBUG

LoopState State;
EffectConfiguration Evaluation;
int SensorReading, currentTime;
Adafruit_NeoPixel hairStrip = Adafruit_NeoPixel(HAIR_NUM_PIXELS, HAIR_LED_PIN);
Adafruit_NeoPixel gloveStrip = Adafruit_NeoPixel(GLOVE_NUM_PIXELS, GLOVE_LED_PIN);
Wrath::WrathEffect WrathEffect = Wrath::WrathEffect(&gloveStrip, GLOVE_NUM_PIXELS);
Lifebloom::LifebloomEffect LifebloomEffect = Lifebloom::LifebloomEffect(&hairStrip, &gloveStrip, GLOVE_NUM_PIXELS);

//For Demo
int ellapsedGap = 0, previousTime = 0, effectGap = 30000; //milliseconds (aka 30 seconds)
bool doLifebloom = true;

void setup() 
{
  // put your setup code here, to run once:
  State = None;
  Evaluation.Duration = 100;
  hairStrip.begin();
  hairStrip.show();
  gloveStrip.begin();
  gloveStrip.show();
  POST();
  #ifdef DEBUG
  Serial.begin(9600);
  #endif
}

void loop() 
{
  currentTime = millis();
  if (State == Done)
  {
    WaitForReset();
  }
  else if (State == None || State == Evaluating)
  {
    //LoopState readResult = DetermineAngle(ReadFlexSensor());
    //if (readResult != None)
    //{
    //  Evaluate(readResult);
    //}
    ellapsedGap += (currentTime - previousTime);
    #ifdef DEBUG
    Serial.print("EllapseGap : ");
    Serial.println(ellapsedGap);
    #endif
    if (ellapsedGap >= effectGap)
    {
      #ifdef DEBUG
      Serial.print("Running ");
      #endif
      ellapsedGap = 0;
      doLifebloom ^= true;
      if (doLifebloom)
      {
        #ifdef DEBUG
        Serial.println("lifebloom");
        #endif
        State = HalfBend;
        LifebloomEffect.Start(currentTime);
      }
      else
      {
        #ifdef DEBUG
        Serial.println("wrath");
        #endif
        State = FullBend;
        WrathEffect.Start(currentTime);
      }
    }
  }
  else if (State == HalfBend || State == FullBend)
  {
    if (State == HalfBend)
    {
      LifebloomEffect.DoEffect(currentTime);
      if (LifebloomEffect.State == Lifebloom::Done)
      {
        State = Done;
        LifebloomEffect.State = Lifebloom::None;
      }
    }
    else
    {
      WrathEffect.DoEffect(currentTime);
      if (WrathEffect.State == Wrath::Done)
      {
        State = Done;
        WrathEffect.State = Wrath::None;
      }
    }
  }
  previousTime = currentTime;
//  #else
//  #ifdef DEBUG
//  Serial.print("Wrath state is ");
//  switch(WrathEffect.State)
//  {
//    case Wrath::None : Serial.println("None"); break;
//    case Wrath::Fade : Serial.println("Fade"); Serial.print("Wrath elapsed time "); Serial.println(WrathEffect.FadeEffect.ElapsedTime(currentTime)); break;
//    case Wrath::Flash : Serial.println("Flash"); Serial.print("Wrath elapsed time "); Serial.println(WrathEffect.FlashEffect.ElapsedTime(currentTime)); break;
//  }
//  Serial.print("Brightness is ");
//  Serial.print(WrathEffect.brightness);
//  Serial.print(" scaled to ");
//  Serial.println((int)(255.0f * WrathEffect.brightness));
//  Serial.print("BrightnessInterval is ");
//  Serial.println(WrathEffect.brightnessInterval);
//  #endif
}

// Power On Self Test
void POST() {
  for (int i = 0; i < HAIR_NUM_PIXELS; i++)
  {
    hairStrip.setPixelColor(i, 255, 0, 0);
    hairStrip.show();
    delay(200);
    hairStrip.setPixelColor(i, 0, 255, 0);
    hairStrip.show();
    delay(200);
    hairStrip.setPixelColor(i, 0, 0, 255);
    hairStrip.show();
    delay(200);
    hairStrip.setPixelColor(i, 0, 0, 0);
    hairStrip.show();
  }
  for (int i = 0; i < GLOVE_NUM_PIXELS; i++)
  {
    gloveStrip.setPixelColor(i, 255, 0, 0);
    gloveStrip.show();
    delay(200);
    gloveStrip.setPixelColor(i, 0, 255, 0);
    gloveStrip.show();
    delay(200);
    gloveStrip.setPixelColor(i, 0, 0, 255);
    gloveStrip.show();
    delay(200);
    gloveStrip.setPixelColor(i, 0, 0, 0);
    gloveStrip.show();
  }
}

int ReadFlexSensor() 
{
  int reading = analogRead(SENSOR_PIN);
  int mappedReading = map(reading, 300, 210, 0, 90);
  #ifdef DEBUG
  Serial.print("Reading is ");
  Serial.print(reading);
  Serial.print(" mapped to ");
  Serial.println(mappedReading);
  #endif
  return mappedReading;
}

LoopState DetermineAngle(int reading)
{
  //reading = 70;
  if (reading < 40)
    return None;  
  if (reading < 80)
    return HalfBend;
  return FullBend;
}

void WaitForReset()
{
   if (DetermineAngle(ReadFlexSensor()) == None)
     State = None;
}

void Evaluate(LoopState readResult)
{
  //If we haven't read anything yet, start Evaluating the bend position if it's interesting
  if (State == None)
  {
    State = Evaluating;
    Evaluation.Start(currentTime);
  }
  else if (State != Done)
  {
    //There is a small grace period before an effect is triggered
    //Once you pass the grace period, start the effect based on which
    //angle range read last
    if (Evaluation.ElapsedTime(currentTime) >= Evaluation.Duration)
    {
      State = readResult;
      if (State == HalfBend)
      {
        LifebloomEffect.Start(currentTime);
      }
      else
      {
        WrathEffect.Start(currentTime);
      }
    }
  }
}

