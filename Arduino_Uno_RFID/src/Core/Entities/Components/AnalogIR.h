#ifndef AnalogIR_h
#define Analog_h
#include "Arduino.h"

//#define DEBUG_AIR

class AnalogIR {
private:

	byte* _address;
	byte _sensorType;

	byte GetDigitalReading();
	byte GetDefenceSensorReading();
	byte GetSpellSensorReading();
	void SetMultiplexerAddress();

public:

	byte CurrentValue;
	
	AnalogIR();
	AnalogIR(byte address[], byte sensorType);

	bool isNewCardPresent();
};

#endif