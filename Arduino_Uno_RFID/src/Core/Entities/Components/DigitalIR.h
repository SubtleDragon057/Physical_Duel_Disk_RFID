#ifndef DigitalIR_h
#define DigitalIR_h
#include "Arduino.h"
#include "Models\ProximitySensor.h"

class DigitalIR : public ProximitySensor {
private:
	byte _pin;
	int _currentValue;

public:

	DigitalIR();
	DigitalIR(uint8_t pin);

	virtual bool isNewCardPresent();
	virtual int GetCurrentValue();
};

#endif