#include "AnalogIR.h"

AnalogIR::AnalogIR() 
{
}
AnalogIR::AnalogIR(byte address[], byte sensorType) {	
	_address = address;
	_sensorType = sensorType;
	CurrentValue = 1; // Enums::HIGH
}

bool AnalogIR::isNewCardPresent() {
	SetMultiplexerAddress();
	int read;

	switch (_sensorType) {
		case 1 :
			read = GetDigitalReading();
			break;
		case 2:
			read = GetDefenceSensorReading();
			break;
		case 3:
			read = GetSpellSensorReading();
			break;
	}

	if (read == CurrentValue) return false;

	CurrentValue = read;
	return true;
}

byte AnalogIR::GetDigitalReading() {
	int read = digitalRead(A0);

#ifdef DEBUG_AIR
	Serial.print(": ");
	Serial.println(read);
#endif // DEBUG_AIR

	return read;
}

byte AnalogIR::GetDefenceSensorReading() {
	int read = 0;

	for (byte i = 10; i > 0; i--) {
		int val = analogRead(A0);
		read += val;
	}
	read = (read / 10);

#ifdef DEBUG_AIR
	Serial.print(": ");
	Serial.println(read);
#endif // DEBUG_AIR	

	if (read < 130) {
		return 0; // Enums::LOW
	}
	else if (read > 550) {
		return 1; // Enums::HIGH
	}

	return 2; // Enums::Medium
}

byte AnalogIR::GetSpellSensorReading() {
	int read = 0;

	for (byte i = 10; i > 0; i--) {
		int val = analogRead(A0);
		read += val;
	}
	read = (read / 10);

#ifdef DEBUG_AIR
	Serial.print(": ");
	Serial.println(read);
#endif // DEBUG_AIR

	if (read < 130) {
		return 0; // Enums::LOW
	}
	else if (read > 800) {
		return 1; // Enums::HIGH
	}

	return 2; // Enums::Medium
}

void AnalogIR::SetMultiplexerAddress() {
	for (byte i = 0; i < 4; i++) {
		digitalWrite(2 + i, _address[i]);
		
#ifdef DEBUG_AIR
		Serial.print(_address[i]);
#endif // DEBUG_AIR

	}
}