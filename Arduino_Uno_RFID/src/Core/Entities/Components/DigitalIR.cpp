#include "DigitalIR.h"

DigitalIR::DigitalIR()
{
}
DigitalIR::DigitalIR(uint8_t pin) {
	_pin = pin;
	_currentValue = HIGH;

	pinMode(_pin, INPUT);
}

bool DigitalIR::isNewCardPresent() {
	int read = digitalRead(_pin);

	if (read == _currentValue) {
		return false;
	}

	_currentValue = read;
	return true;
}

int DigitalIR::GetCurrentValue() {
	_currentValue = digitalRead(_pin);
	return _currentValue;
}