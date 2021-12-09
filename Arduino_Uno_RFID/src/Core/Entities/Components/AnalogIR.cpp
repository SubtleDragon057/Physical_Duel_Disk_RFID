#include "AnalogIR.h"

AnalogIR::AnalogIR() {

}
AnalogIR::AnalogIR(uint8_t pin, bool debug) {
	_pin = pin;
	_currentValue = 2; // Enums::HIGH
	_debug = debug;
}

int AnalogIR::GetCurrentValue() {
	_currentValue = GetSensorReading();
	return _currentValue;
}

bool AnalogIR::isNewCardPresent() {
	int read = GetSensorReading();

	if (read == _currentValue) {
		return false;
	}

	_currentValue = read;
	return true;
}

int AnalogIR::GetSensorReading() {
	int read = analogRead(_pin);

	if (read < 100) {
		return 0; // Enums::LOW
	}
	else if (read > 550) {
		return 2; // Enums::HIGH
	}

	return 1; // Enums::Medium
}