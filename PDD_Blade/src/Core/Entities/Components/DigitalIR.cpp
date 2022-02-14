#include "DigitalIR.h"

DigitalIR::DigitalIR()
{
}
DigitalIR::DigitalIR(byte address[]) {
	_address = address;
	_currentValue = HIGH;
}

bool DigitalIR::isNewCardPresent() {
	SetMultiplexerAddress();
	int read = digitalRead(A0);

	if (read == _currentValue) {
		return false;
	}

	_currentValue = read;
	return true;
}

int DigitalIR::GetCurrentValue() {
	SetMultiplexerAddress();

	_currentValue = digitalRead(A0);
	return _currentValue;
}

void DigitalIR::SetMultiplexerAddress()
{
	for (byte i = 0; i < 4; i++) {
		digitalWrite(2 + i, _address[i]);
	}
}