#include "AnalogIR.h"

AnalogIR::AnalogIR() 
{
}
AnalogIR::AnalogIR(byte address[], byte sensorType, bool debug) {	
	_address = address;
	_sensorType = sensorType;
	CurrentValue = 1; // Enums::HIGH
	_debug = debug;
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

	if (read == CurrentValue) {
		return false;
	}

	CurrentValue = read;
	return true;
}

byte AnalogIR::GetDigitalReading() {
	int read = digitalRead(A0);

	if (_debug) {
		Serial.print(": ");
		Serial.println(read);
		delay(25);
	}

	return read;
}

byte AnalogIR::GetDefenceSensorReading() {
	int read = 0;

	for (byte i = 10; i > 0; i--) {
		int val = analogRead(A0);
		read += val;
	}
	read = (read / 10);

	if (_debug) {
		Serial.print(": ");
		Serial.println(read);
		delay(25);
	}	

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

	if (_debug) {
		Serial.print(": ");
		Serial.println(read);
		delay(25);
	}

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
		
		if (_debug) {
			Serial.print(_address[i]);
			delay(25);
		}
	}
}