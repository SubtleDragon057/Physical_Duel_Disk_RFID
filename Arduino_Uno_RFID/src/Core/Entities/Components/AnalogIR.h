#ifndef AnalogIR_h
#define Analog_h
#include "Arduino.h"

class AnalogIR {
private:

	byte* _address;
	byte _sensorType;

	bool _debug;

	byte GetDigitalReading();
	byte GetDefenceSensorReading();
	byte GetSpellSensorReading();
	void SetMultiplexerAddress();

public:

	byte CurrentValue;
	
	AnalogIR();
	AnalogIR(byte address[], byte sensorType, bool debug = false);

	bool isNewCardPresent();
};

#endif