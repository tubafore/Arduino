#ifndef __IRCODEC_H
#define __IRCODEC_H

class IRCodec
{
private:
	unsigned short receivePin;
	unsigned short transmitPin;
public:

	IRCodec();

	void SetReceivePin(unsigned short pin);
	void SetTransmitPin(unsigned short pin);
	unsigned short GetReceivePin();
	unsigned short GetTransmitPin();

	unsigned int ReceiveMessage();
	void         TransmitMessage(int format, unsigned int message, int standardInterval, int highMultiplier);
	unsigned int ReverseBytes(unsigned int message);
};

#define NEC 1

#endif