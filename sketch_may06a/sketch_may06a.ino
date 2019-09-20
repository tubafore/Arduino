int datapin = 2; 
int clockpin = 3;
int latchpin = 4;

// We'll also declare a global variable for the data we're
// sending to the shift register:

byte data = 0;
byte timeArray[8] = {168, 192, 16, 255, 255, 0, 128, 220 };//{2, 4, 8, 16, 32, 64, 128, 255 };
byte remainingTimeArray[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
int i, j, k;

void setup()
{
  // Set the three SPI pins to be outputs:

  pinMode(datapin, OUTPUT);
  pinMode(clockpin, OUTPUT);  
  pinMode(latchpin, OUTPUT);
  data = 0xFF;
}


void loop()
{
  for (i = 0; i < 8; i++)
  {
    remainingTimeArray[i] = timeArray[i];
  }
  for (j = 0; j < 255; j++)
  {
    for (k = 0; k < 8; k++)
    { 
      if (remainingTimeArray[k] > 0)
      {
        remainingTimeArray[k]--;
        bitSet(data, k);
      }        
      else
        bitClear(data, k);
    }

    digitalWrite(latchpin, HIGH);
    shiftOut(datapin, clockpin, LSBFIRST, data);
    digitalWrite(latchpin, LOW);
  }
}
