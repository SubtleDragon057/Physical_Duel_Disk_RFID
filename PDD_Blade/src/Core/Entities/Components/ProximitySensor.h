#ifndef ProximitySensor_h
#define ProximitySensor_h
#include "Arduino.h"

//#define DEBUG_AIR

class ProximitySensor {
private:

	byte* _address;
	byte _sensorType;

	byte GetDigitalReading();
	byte GetDefenceSensorReading();
	byte GetSpellSensorReading();
	void SetMultiplexerAddress();

public:

	byte CurrentValue;
	
	ProximitySensor();
	ProximitySensor(byte address[], byte sensorType);

	bool isNewCardPresent();
};

#endif