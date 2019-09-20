#include <Adafruit_NeoPixel.h>

namespace Wrath {

  enum WrathState
  {
    None,
    Flash,
    Fade,
    Done
  };
  
  struct WrathEffect
  {
    EffectConfiguration FlashEffect;
    EffectConfiguration FadeEffect;
    WrathState State;
    Adafruit_NeoPixel *strip;
    float brightnessInterval;
    float maxBrightness = 255.0f;
    float brightness = 0.0f;
    int pixelCount = 7;
  
    WrathEffect(Adafruit_NeoPixel *pixels, int numPixels)
    {
      State = None;
      FlashEffect.Duration = 200;
      FlashEffect.DelayTime = 10;
      FadeEffect.Duration = 1000;
      FadeEffect.DelayTime = 10;
      strip = pixels;
      pixelCount = numPixels;
    }
  
    void Start(int currentTime)
    {
      if (State == None)
        StartFlash(currentTime);
      else if (State == Flash)
        StartFade(currentTime);
    }

    void StartFlash(int currentTime)
    {
      FlashEffect.Start(currentTime);
      State = Flash;
      brightnessInterval = FlashEffect.BrightnessInterval();
      brightness = 0.0f;
    }

    void StartFade(int currentTime)
    {
      FadeEffect.Start(currentTime);
      State = Fade;
      brightnessInterval = FadeEffect.BrightnessInterval();
      brightness = 1.0f;
    }
  
    void DoEffect(int currentTime)
    {
       int red, green, blue;
       if (State == None)
         return;
       if (State == Flash)
       {
         if (FlashEffect.ElapsedTime(currentTime) > FlashEffect.DelayTime)
         {
           green = (int)(255.0f * brightness);
           red = (int)(255.0f * brightness);
           blue = 0;
           if (green > 255)
             green = 255;
           for (int i = 0; i < pixelCount; i++)
           {
             strip->setPixelColor(i, red, green, blue);
           }
           brightness += brightnessInterval;
           FlashEffect.PreviousEffectTime = currentTime;
         }
         if (brightness >= 1.0)
         {
           Start(currentTime);
         }
       }
       else
       {
         if (FadeEffect.ElapsedTime(currentTime) > FadeEffect.DelayTime)
         {
           green = (int)(255.0f * brightness);
           red = (int)(255.0f * brightness);
           blue = 0;
           if (green < 0)
             green = 0;
           for (int i = 0; i < pixelCount; i++)
           {
             strip->setPixelColor(i, red, green, blue);
           }
           brightness -= brightnessInterval;
           FadeEffect.PreviousEffectTime = currentTime;
         }
         if (brightness <= 0.0f)
         {
           State = Done;
         }
       }
       strip->show();
    }
  };
}
