#ifndef ProximitySensor_h
#define ProximitySensor_h
#include "Arduino.h"

class ProximitySensor {
public:
	enum SensorType {
		Attack,
		Defence,
		Spell
	};
	enum SensorReading {
		Low = 0,
		Medium = 2,
		High = 1
	};

	byte CurrentValue = NoCard;

	ProximitySensor(SensorType sensorType);

	void UpdateAddress(byte address[]);
	bool isCardPresent();

private:

	byte _address[4] = { 0, 0, 0, 0 };
	SensorType _sensorType;

	enum CardPresence {
		FaceUp = 0, // Enums::LOW
		NoCard = 1, // Enums::HIGH
		FaceDown = 2 // Enums::Medium
	};

	byte GetCurrentPosition();
	float GetReading();
	void SetMultiplexerAddress();
};

#endif