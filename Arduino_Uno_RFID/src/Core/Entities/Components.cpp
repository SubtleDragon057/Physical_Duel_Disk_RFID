#include "Arduino.h"
#include "Components.h"
#include "Enums.h"

ProximitySensor::ProximitySensor() 
{
}
ProximitySensor::ProximitySensor(uint8_t pin, String name) {
	_pin = pin;
	_currentValue = HIGH;
	_name = name;

	pinMode(_pin, INPUT);
}

bool ProximitySensor::isNewCardPresent() {
	int read = digitalRead(_pin);

	if (read == _currentValue) {
		return false;
	}

	_currentValue = read;
	return true;
}

int ProximitySensor::GetCurrentValue() {
	_currentValue = digitalRead(_pin);
	return _currentValue;
}

AnalogIR::AnalogIR() {

}
AnalogIR::AnalogIR(uint8_t pin, String name, bool debug) {
	_pin = pin;
	_currentValue = Enums::High;
	_name = name;
	_debug = debug;
}

int AnalogIR::GetCurrentValue()
{	
	_currentValue = GetSensorReading();
	return _currentValue;
}

bool AnalogIR::isNewCardPresent() {
	int read = GetSensorReading();
	
	if (_debug) {
		Serial.print("Current Value: ");
		Serial.println(_currentValue);
		Serial.print("Read Value: ");
		Serial.println(read);
		delay(50);
	}

	if (read == _currentValue) {
		return false;
	}

	_currentValue = read;
	return true;
}

int AnalogIR::GetSensorReading() {
	int read = analogRead(_pin);
	
	if (_debug) {
		Serial.print("Name: ");
		Serial.println(_name);
		Serial.print("Analog Val: ");
		Serial.println(read);
		delay(50);
	}

	if (read < 100) {
		return Enums::Low;
	}
	else if (read > 550) {		
		return Enums::High;
	}
	
	return Enums::Medium;
}