#ifndef Components_h
#define Components_h
#include "Arduino.h"

class ProximitySensor {
public:	
	virtual bool isNewCardPresent() = 0;
	virtual int GetCurrentValue() = 0;
};

#endif