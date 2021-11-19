#ifndef Components_h
#define Components_h
#include "Arduino.h"
#include "Enums.h"

class ProximitySensor {
protected:
	uint8_t _pin;
	int _currentValue;
	String _name;

public:

	ProximitySensor();
	ProximitySensor(uint8_t pin, String name);
	virtual bool isNewCardPresent();
	virtual int GetCurrentValue();
};

class AnalogIR : public ProximitySensor {
private:
	int GetSensorReading();
	bool _debug;

public:
	
	AnalogIR();
	AnalogIR(uint8_t pin, String name, bool debug);
	
	int GetCurrentValue() override;
	bool isNewCardPresent() override;
};

#endif
