byte data;
const byte PWMInfo[8] = { 0, 64, 96, 128, 160, 192, 224, 256 };
byte remainingTime[8];
const int dataPin = 2;
const int clockPin = 3;
const int latchPin = 4;
bool anyZeros = false;
int i = 0, j = 0;

#define TickClock PORTD ^= 0x08;
#define SetLatch PORTD |= 0x10;
#define ClearLatch PORTD &= 0xEF;
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
}

void loop() {
	// put your main code here, to run repeatedly:
	SetTimes();
	LightEmUp();
}

void LightEmUp() {
	SetLatch;
	//anyZeros = false;

	for (j = 0; j < 256; j++)
	{
		WriteByte(0x80);
	}
	ClearLatch;
	//if (!anyZeros)
	//  SetTimes();
}

void SetTimes() {
	for (i = 0; i < 8; i++)
		remainingTime[i] = PWMInfo[i];
}

void WriteByte(byte value)
{
	SetLatch;
	for (i = 0; i < 8; i++)
	{
		bitWrite(PORTD, 2, 0);
		TickClock;
	}
	for (i = 0; i < 8; i++)
	{
		bitWrite(PORTD, 2, (value >> i % 2));
		TickClock;
	}
	ClearLatch;
}