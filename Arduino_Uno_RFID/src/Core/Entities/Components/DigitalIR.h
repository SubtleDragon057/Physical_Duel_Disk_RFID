#ifndef DigitalIR_h
#define DigitalIR_h
#include "Arduino.h"

class DigitalIR {
private:
	byte* _address;
	int _currentValue;

	void SetMultiplexerAddress();

public:

	DigitalIR();
	DigitalIR(byte address[]);

	bool isNewCardPresent();
	int GetCurrentValue();
};

#endif