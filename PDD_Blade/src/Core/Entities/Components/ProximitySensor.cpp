#include "ProximitySensor.h"

//#define DEBUG_PS

ProximitySensor::ProximitySensor(SensorType sensorType) {
	_sensorType = sensorType;
}

void ProximitySensor::UpdateAddress(byte address[]) {
	for (byte i = 0; i < 4; i++) {
		_address[i] = address[i];
	}
}

bool ProximitySensor::isCardPresent() {
	SetMultiplexerAddress();
	CurrentValue = GetCurrentPosition();

	return CurrentValue != NoCard;
}

byte ProximitySensor::GetCurrentPosition() {
	float read = GetReading();

	if (read < 0) { read = 0; }
	else if (read > 1000) { read = 1000; }

	if (read < 150) {
		return FaceDown;
	}
	else if (read > 500) {
		return NoCard;
	}

	return FaceUp;
}

float ProximitySensor::GetReading() {
	float averageValue = 0;
	uint16_t maxSensorValue = 0;
	uint16_t minSensorValue = 1000;
	
	for (uint8_t i = 0; i < 12; i++) {
		uint16_t sensorValue = 0;

		for (uint8_t j = 0; j < 4; j++) {
			sensorValue += analogRead(A0);
		}
		sensorValue = sensorValue / 4;

		if (sensorValue > maxSensorValue) {
			maxSensorValue = sensorValue;
		}

		if (sensorValue < minSensorValue) {
			minSensorValue = sensorValue;
		}

		averageValue += sensorValue;
	}

	uint16_t outlyingResults = maxSensorValue + minSensorValue;
	averageValue = (averageValue - outlyingResults) / 10;

#ifdef DEBUG_PS
	Serial.print("Average Value: "); Serial.println(averageValue);
	Serial.print("Highest Value: "); Serial.println(maxSensorValue);
	Serial.print("Lowest Value: "); Serial.println(minSensorValue);
#endif

	return averageValue;
}

void ProximitySensor::SetMultiplexerAddress() {
	for (byte i = 0; i < 4; i++) {
		digitalWrite(2 + i, _address[i]);
	}
}