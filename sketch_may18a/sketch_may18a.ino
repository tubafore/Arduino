byte data;
const byte PWMInfo[8] = { 0, 0, 0, 0, 32, 0, 0,  1};
byte remainingTime[8];
const int dataPin = 2;
const int clockPin = 3;
const int latchPin = 4;
bool anyZeros = false;
int i = 0, j = 0;

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif 
#ifndef tick
#define tick(sfr, bit) (_SFR_BYTE(sfr) ^= _BV(bit));
#endif

//#define TickClock PORTD ^= 0x08;
//#define SetLatch PORTD |= 0x10;
//#define ClearLatch PORTD &= 0xEF;
#define TickClock tick(PORTD, 3);
#define SetLatch sbi(PORTD, 4);
#define ClearLatch cbi(PORTD, 4);
#define PIN0 0
#define PIN1 2
#define PIN2 4
#define PIN3 6
#define PIN4 8
#define PIN5 10
#define PIN6 12
#define PIN7 14

void setup() {
	//put your setup code here, to run once:
  DDRD = 0x1C;
  data = 0xFF;
  WriteByte(0);
  WriteByte(0);
}

void loop() {
	// put your main code here, to run repeatedly:
  SetTimes();
  LightEmUp();
}

void LightEmUp() {
  for (j = 0; j < 256; j++)
  {
    data = 0;
    for (i = 0; i < 8; i++)
    {
      bitWrite(data, i, j < PWMInfo[i]);
    }
    WriteByte(data);
  }
}

void SetTimes() {
  for (i = 0; i < 8; i++)
    remainingTime[i] = PWMInfo[i];
}

void WriteByte(byte value)
{
  ClearLatch;
  for (i = 0; i < 8; i++)
  { 
    TickClock;
    if (value & (0x1 << i))
      sbi(PORTD, 2);
    else
      cbi(PORTD, 2);
    TickClock;
  }
  SetLatch;
}
