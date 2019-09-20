#include <stdint.h>

enum LoopState 
{
  None,
  Evaluating,
  HalfBend,
  FullBend,
  Done
};

struct EffectConfiguration
{
  uint32_t Duration;
  uint32_t DelayTime;
  uint32_t StartTime;
  uint32_t PreviousEffectTime;
  float maxBrightness = 255.0f;

  void Start(int currentTime)
  {
    PreviousEffectTime = currentTime;
    StartTime = currentTime;
  }
  
  uint32_t ElapsedTime(int currentTime)
  {
    if (currentTime < PreviousEffectTime)
      PreviousEffectTime = currentTime;
    return currentTime - PreviousEffectTime;
  }

  float BrightnessInterval()
  {
    return ((float)DelayTime)/((float)Duration);
  }
};

