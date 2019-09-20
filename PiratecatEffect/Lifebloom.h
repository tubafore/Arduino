#include <Adafruit_NeoPixel.h>

namespace Lifebloom {

  enum LifebloomState
  {
    None,
    Subset1Flash,
    Subset1Fade,
    Subset2Flash,
    Subset2Fade,
    Subset3Flash,
    Subset3Fade,
    Flash,
    Fade,
    Done
  };
  
  struct LifebloomEffect
  {
    EffectConfiguration SubsetFlash;
    EffectConfiguration SubsetFade;
    EffectConfiguration FlashEffect;
    EffectConfiguration FadeEffect;
    int subset[3][6] = { { 0, 2, 4, 6, 8, 10},
                         { 1, 3, 5, 7, 9, 11},
                         { 0, 2, 4, 6, 8, 10} };
    int subsetLength = 6;
    int fullLength = 12;
    int handPixels;
    LifebloomState State;
    Adafruit_NeoPixel *strip;
    Adafruit_NeoPixel *handStrip;
    float brightnessInterval;
    float brightness = 0.0f;
    
    LifebloomEffect(Adafruit_NeoPixel *pixels, Adafruit_NeoPixel *hand, int handPixelCount)
    {
      State = None;
      SubsetFlash.Duration = 200;
      SubsetFlash.DelayTime = 10;
      SubsetFade.Duration = 1000;
      SubsetFade.DelayTime = 10;
      FlashEffect.Duration = 200;
      FlashEffect.DelayTime = 10;
      FadeEffect.Duration = 1000;
      FadeEffect.DelayTime = 10;
      strip = pixels;
      handStrip = hand;
      handPixels = handPixelCount;
    }
  
    void Start(int currentTime)
    {
      if (State == None)
        StartFlash(currentTime, 1);
      else if (State == Subset1Flash)
        StartFade(currentTime, 1);
      else if (State == Subset1Fade)
        StartFlash(currentTime, 2);
      else if (State == Subset2Flash)
        StartFade(currentTime, 2);
      else if (State == Subset2Fade)
        StartFlash(currentTime, 3);
      else if (State == Subset3Flash)
        StartFade(currentTime, 3);
      else if (State == Subset3Fade)
        StartFlash(currentTime, 0);
      else if (State == Flash)
        StartFade(currentTime, 0);
    }

    void StartFlash(int currentTime, int set)
    {
      if (set == 0)
      {
        FlashEffect.Start(currentTime);
        brightnessInterval = FlashEffect.BrightnessInterval();
        State = Flash;
      }
      else
      {
        SubsetFlash.Start(currentTime);
        brightnessInterval = SubsetFlash.BrightnessInterval();
        if (set == 1)
          State = Subset1Flash;
        else if (set == 2)
          State = Subset2Flash;
        else
          State = Subset3Flash;
      }
      
      brightness = 0.0f;
    }

    void StartFade(int currentTime, int set)
    {
      if (set == 0)
      {
        FadeEffect.Start(currentTime);
        State = Fade;
        brightnessInterval = FadeEffect.BrightnessInterval();
      }
      else
      {
        SubsetFade.Start(currentTime);
        brightnessInterval = SubsetFade.BrightnessInterval();
        if (set == 1)
          State = Subset1Fade;
        else if (set == 2)
          State = Subset2Fade;
        else
          State = Subset3Fade;
      }
      brightness = 1.0f;
    }

    int GetSubsetIndex()
    {
      switch (State)
      {
        case Flash:
        case Fade:
          return -1;
        case Subset1Flash:
        case Subset1Fade:
          return 0;
        case Subset2Flash:
        case Subset2Fade:
          return 1;
        case Subset3Flash:
        case Subset3Fade:
          return 2;
      }
    }
  
    void DoEffect(int currentTime)
    {
      if (State == None)
        return;
      
      int red, green, blue;
      bool DelayReached = false;
      int pixelIndex = GetSubsetIndex();
      float maxBrightness;
      int maxBrightnessAsInt;
      if (pixelIndex == -1)
      {
        maxBrightness = 255.0f;
        maxBrightnessAsInt = 255;
      }
      else
      {
        maxBrightness = 128.0f;
        maxBrightnessAsInt = 128;
      }
      
      if (State == Flash || State == Subset1Flash || State == Subset2Flash || State == Subset3Flash)
      {
        if (pixelIndex == -1)
          DelayReached = (FlashEffect.ElapsedTime(currentTime) > FlashEffect.DelayTime);
        else
          DelayReached = (SubsetFlash.ElapsedTime(currentTime) > SubsetFlash.DelayTime);
         
        if (DelayReached)
        {
          green = (int)(maxBrightness * brightness);
          red = 0;
          blue = 0;
          if (green > maxBrightnessAsInt)
            green = maxBrightnessAsInt;
        
          if (pixelIndex == -1)
          {
            for (int i = 0; i < fullLength; i++)
            {
              strip->setPixelColor(i, red, green, blue);
            }
            FlashEffect.PreviousEffectTime = currentTime;
          }
          else
          {
            for (int i = 0; i < subsetLength; i++)
            {
              strip->setPixelColor(subset[pixelIndex][i], red, green, blue);
            }
            if (pixelIndex == 0)
            {
              green = (int)(255 * brightness);
              if (green > 255)
                green = 255;
              for (int i = 0; i < handPixels; i++)
              {
                handStrip->setPixelColor(i, red, green, blue);
              }
            }
            SubsetFlash.PreviousEffectTime = currentTime;
          }
          brightness += brightnessInterval;
        }
        if (brightness >= 1.0)
        {
          Start(currentTime);
        }
      }
      else
      {
        if (pixelIndex == -1)
          DelayReached = (FadeEffect.ElapsedTime(currentTime) > FadeEffect.DelayTime);
        else
          DelayReached = (SubsetFade.ElapsedTime(currentTime) > SubsetFade.DelayTime);
         
        if (DelayReached)
        {
          green = (int)(maxBrightness * brightness);
          red = 0;
          blue = 0;
          if (green < 0)
            green = 0;
        
          if (pixelIndex == -1)
          {
            for (int i = 0; i < fullLength; i++)
            {
              strip->setPixelColor(i, red, green, blue);
            }
            FadeEffect.PreviousEffectTime = currentTime;
          }
          else
          {
            for (int i = 0; i < subsetLength; i++)
            {
              strip->setPixelColor(subset[pixelIndex][i], red, green, blue);
            }
            if (pixelIndex == 0)
            {
              green = (int)(255 * brightness);
              if (green < 0)
                green = 0;
              for (int i = 0; i < handPixels; i++)
              {
                handStrip->setPixelColor(i, red, green, blue);
              }
            }
            SubsetFade.PreviousEffectTime = currentTime;
          }
          brightness -= brightnessInterval;
        }
        if (brightness <= 0.0)
        {
          Start(currentTime);
          if (pixelIndex == -1)
            State = Done;
        }
      }
      strip->show();
      if (pixelIndex == 0)
        handStrip->show();
    }
  };
}
