#ifndef _ZONE_h
#define _ZONE_h
#include "arduino.h"
#include "MFRC522.h"

class Zone {
public:

	enum LightValue {
		Battle,
		Face
	};

	class ProximitySensor {
	public:
		byte pin;
		int currentValue;

		ProximitySensor();
		ProximitySensor(byte pinNumber);
	};

	class Monster {
	public:
		enum Modes {
			Attack,
			Defence,
			FaceDown,
			FaceUp
		};

		String serialNumber;
		int battleMode;
		int faceMode;

		Monster();
	};

	int zoneNumber;
	MFRC522 reader;
	ProximitySensor sensor;
	int battleLightPin;
	int faceLightPin;
	Monster monster;

	Zone();
	Zone(int zoneNum, MFRC522 mfrc522, ProximitySensor proximitySensor, int battleLightPin, int faceLightPin);

	bool CheckForNewCard();
	void UpdateCurrentMonster(String monsterID);
	void UpdateCurrentSpell(String spellID);
	Monster GetCurrentMonster();
	String GetCurrentSpell();
	int GetLightValue(LightValue value);

private:
	Monster _currentMonster;
	String _currentSpell;
	int _currentBattleLightValue;
	int _currentFaceLightValue;
};

#endif