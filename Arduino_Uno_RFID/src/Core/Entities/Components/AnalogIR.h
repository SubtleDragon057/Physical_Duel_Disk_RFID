#ifndef AnalogIR_h
#define Analog_h
#include "Arduino.h"

//#define DEBUG_AIR

// TODO: Fix class name & remove Digital IR from solution
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