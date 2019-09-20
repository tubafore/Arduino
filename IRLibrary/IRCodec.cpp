#include "IRCodec.h"


IRCodec::IRCodec()
{

}

unsigned short IRCodec::GetReceivePin()
{
	return receivePin;
}

unsigned short IRCodec::GetTransmitPin()
{
	return transmitPin;
}

void IRCodec::SetReceivePin(unsigned short pin)
{
	receivePin = pin;
}

void IRCodec::SetTransmitPin(unsigned short pin)
{
	transmitPin = pin;
}

unsigned int IRCodec::ReverseBytes(unsigned int message)
{
	unsigned int result = 0;

	for (short i = 0; i < 32; i++)
	{
		result <<= 1;
		result |= message & 0x1;
		message >>= 1;
	}

	return result;
}

void IRCodec::TransmitMessage(int format, unsigned int message, int standardInterval, int highMultiplier)
{
	cli();
	switch (format)
	{
		case NEC: 
			digitalWrite(transmitPin, HIGH); 
			delayMicroseconds(32 * standardInterval); 
			digitalWrite(transmitPin, LOW); 
			delayMicroseconds(16 * standardInterval); 
			break;
	}

	message = ReverseBytes(message);

	for (short i = 0; i < 32; i++)
	{
		digitalWrite(transmitPin, HIGH);
		delayMicroseconds(2 * standardInterval);
		digitalWrite(transmitPin, LOW);
		if (message | 0x1)
		{
			delayMicroseconds(6 * standardInterval);
		}
		else
		{
			delayMicroseconds(2 * standardInterval);
		}
		message >>= 1;
	}

	switch (format)
	{
		case NEC:
			digitalWrite(transmitPin, HIGH);
			delayMicroseconds(2 * standardInterval);
			digitalWrite(transmitPin, LOW);
			delayMicroseconds(144 * standardInterval);
			digitalWrite(transmitPin, HIGH);
			delayMicroseconds(32 * standardInterval);
			digitalWrite(transmitPin, LOW);
			delayMicroseconds(8 * standardInterval);
			digitalWrite(transmitPin, HIGH);
			delayMicroseconds(2 * standardInterval);
			break;
	}

	digitalWrite(transmitPin, LOW);

	sei();
}