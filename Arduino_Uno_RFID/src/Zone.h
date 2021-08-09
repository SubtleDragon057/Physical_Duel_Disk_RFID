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
	private:
		int _currentValue;

	public:
		byte pin;		

		ProximitySensor();
		void ProximitySensor_Init(byte pinNumber);
		bool CheckForNewCard();
	};

	class Monster {
	private:
		String _serialNumber;
		int _battleMode;
		int _faceMode;
	
	public:
		enum Modes {
			Attack,
			Defence,
			FaceDown,
			FaceUp
		};

		Monster();
		Monster(String serialNum, int battleMode, int faceMode);

		String GetSerialNumber();
		int GetMode(LightValue mode);
		void UpdateMonsterSerial(String serialNumber);
		void UpdateMonsterMode(LightValue mode, Modes newMode);
	};

	int zoneNumber;
	MFRC522 reader;
	ProximitySensor sensor;
	int battleLightPin;
	int faceLightPin;

	Zone();
	void Zone_Init(int zoneNum, MFRC522 mfrc522, ProximitySensor proximitySensor, int battleLightPin, int faceLightPin);

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