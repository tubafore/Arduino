/*
 * ShiftPWM non-blocking RGB fades example, (c) Elco Jacobs, updated August 2012.
 *
 * This example for ShiftPWM shows how to control your LED's in a non-blocking way: no delay loops.
 * This example receives a number from the serial port to set the fading mode. Instead you can also read buttons or sensors.
 * It uses the millis() function to create fades. The block fades example might be easier to understand, so start there.
 *
 * Please go to www.elcojacobs.com/shiftpwm for documentation, fuction reference and schematics.
 * If you want to use ShiftPWM with LED strips or high power LED's, visit the shop for boards.
 */

// ShiftPWM uses timer1 by default. To use a different timer, before '#include <ShiftPWM.h>', add
// #define SHIFTPWM_USE_TIMER2  // for Arduino Uno and earlier (Atmega328)
// #define SHIFTPWM_USE_TIMER3  // for Arduino Micro/Leonardo (Atmega32u4)

// Clock and data pins are pins from the hardware SPI, you cannot choose them yourself.
// Data pin is MOSI (Uno and earlier: 11, Leonardo: ICSP 4, Mega: 51, Teensy 2.0: 2, Teensy 2.0++: 22) 
// Clock pin is SCK (Uno and earlier: 13, Leonardo: ICSP 3, Mega: 52, Teensy 2.0: 1, Teensy 2.0++: 21)

// You can choose the latch pin yourself.
const int ShiftPWM_latchPin=4;

// ** uncomment this part to NOT use the SPI port and change the pin numbers. This is 2.5x slower **
#define SHIFTPWM_NOSPI
const int ShiftPWM_dataPin = 2;
const int ShiftPWM_clockPin = 3;


// If your LED's turn on if the pin is low, set this to true, otherwise set it to false.
const bool ShiftPWM_invertOutputs = false;

// You can enable the option below to shift the PWM phase of each shift register by 8 compared to the previous.
// This will slightly increase the interrupt load, but will prevent all PWM signals from becoming high at the same time.
// This will be a bit easier on your power supply, because the current peaks are distributed.
const bool ShiftPWM_balanceLoad = false;

#include <ShiftPWM.h>

//int datapin = 2; 
//int clockpin = 3;
//int latchpin = 4;

// We'll also declare a global variable for the data we're
// sending to the shift register:

unsigned char maxBrightness = 255;
unsigned char pwmFrequency = 75;
unsigned int numRegisters = 1;
unsigned int numOutputs = 8;
unsigned int numRGBLeds = 2;
unsigned int fadingMode = 0; //start with all LED's off.


byte data = B00000000;
byte timeArray[8] = {168, 192, 16, 255, 255, 0, 128, 220 };//{2, 4, 8, 16, 32, 64, 128, 255 };
unsigned long counter = 0;


void setup()
{
  // Set the three SPI pins to be outputs:
  
  ShiftPWM.SetAmountOfRegisters(numRegisters);

  // SetPinGrouping allows flexibility in LED setup. 
  // If your LED's are connected like this: RRRRGGGGBBBBRRRRGGGGBBBB, use SetPinGrouping(4).
  ShiftPWM.SetPinGrouping(1); //This is the default, but I added here to demonstrate how to use the funtion
  
  ShiftPWM.Start(pwmFrequency,maxBrightness);

//  pinMode(datapin, OUTPUT);
//  pinMode(clockpin, OUTPUT);  
//  pinMode(latchpin, OUTPUT);
}


void loop()
{
    AlternatingFade();
}

void AlternatingFade() 
{
   for (int i = 0; i < 255; i++)
   {
     ShiftPWM.SetOne(0, i);
     ShiftPWM.SetOne(1, 255-i);
     ShiftPWM.SetOne(2, i);
     ShiftPWM.SetOne(3, 255-i);
     ShiftPWM.SetOne(4, i);
     ShiftPWM.SetOne(5, 255-i);
     ShiftPWM.SetOne(6, i);
     ShiftPWM.SetOne(7, 255-i);
     delay(4);
   }
   for (int i = 255; i >= 0; i--)
   {
     ShiftPWM.SetOne(0, i);
     ShiftPWM.SetOne(1, 255-i);
     ShiftPWM.SetOne(2, i);
     ShiftPWM.SetOne(3, 255-i);
     ShiftPWM.SetOne(4, i);
     ShiftPWM.SetOne(5, 255-i);
     ShiftPWM.SetOne(6, i);
     ShiftPWM.SetOne(7, 255-i);
     delay(4);
   }
}
	
