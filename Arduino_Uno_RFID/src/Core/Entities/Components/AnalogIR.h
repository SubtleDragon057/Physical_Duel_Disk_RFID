#ifndef AnalogIR_h
#define Analog_h
#include "Arduino.h"
#include "Models\ProximitySensor.h"

class AnalogIR : public ProximitySensor {
private:
	byte _pin;
	int _currentValue;
	bool _debug;

	int GetSensorReading();

public:

	AnalogIR();
	AnalogIR(uint8_t pin, bool debug);

	virtual int GetCurrentValue();
	virtual bool isNewCardPresent();
};

#endif