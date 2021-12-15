#ifndef AnalogIR_h
#define Analog_h
#include "Arduino.h"

class AnalogIR {
private:

	byte* _address;
	byte _currentValue;
	bool _isDigital = false;
	bool _debug;

	byte GetDigitalReading();
	byte GetAnalogReading();
	void SetMultiplexerAddress();

public:

	AnalogIR();
	AnalogIR(byte address[], bool isDigital = false, bool debug = false);

	byte GetCurrentValue();
	bool isNewCardPresent();
};

#endif