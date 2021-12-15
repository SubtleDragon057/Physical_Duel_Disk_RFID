#include "AnalogIR.h"

AnalogIR::AnalogIR() {

}
AnalogIR::AnalogIR(byte address[], bool isDigital, bool debug) {	
	_address = address;
	_isDigital = isDigital;
	_currentValue = 2; // Enums::HIGH
	_debug = debug;
}

byte AnalogIR::GetCurrentValue() {
	_currentValue = GetAnalogReading();
	return _currentValue;
}

bool AnalogIR::isNewCardPresent() {
	SetMultiplexerAddress();
	int read = _isDigital ? GetDigitalReading() : GetAnalogReading();

	if (read == _currentValue) {
		return false;
	}

	_currentValue = read;
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

byte AnalogIR::GetAnalogReading() {
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
	else if (read > 650) {
		return 2; // Enums::HIGH
	}

	return 1; // Enums::Medium
}

void AnalogIR::SetMultiplexerAddress() {
	for (byte i = 0; i < 4; i++) {
		digitalWrite(2 + i, _address[i]);
		/*Serial.print(_address[i]);
		delay(25);*/
	}
}