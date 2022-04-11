#ifndef Zones_h
#define Zones_h
#include "Arduino.h"
#include "ProximitySensor.h"
#include "PN532.h"

class Zone {
protected:
	struct YGOCard {
		bool isCardPresent = false;
		bool hasCardPresenceChanged = false;
		int serialNum;
		int copyNum = 1;
		String cardPosition;
	};

	Zone() {};

public:
	String ZoneName;
	
	Zone(String zoneName);
};

class SingleCardZone : public Zone {
public:
	YGOCard Card;
	
	SingleCardZone(String zoneName = "");

	int CurrentCard() { return Card.serialNum; }
	int CopyNumber() { return Card.copyNum; }
	String Position() { return Card.cardPosition; }
	void UpdateCurrentCard(int serialNum, int copyNum, String position);
};

class DualCardZone : public Zone {
private:
	PN532 _reader;
	byte _block = 4;
	byte _uid[7];
	byte _uidLength;

	const uint8_t _multiplexerAddress = 0x70;

	ProximitySensor _attackSensor = ProximitySensor(ProximitySensor::Attack);
	ProximitySensor _defenceSensor = ProximitySensor(ProximitySensor::Defence);
	ProximitySensor _spellSensor = ProximitySensor(ProximitySensor::Spell);

	ProximitySensor* _sensors[3] = {
		&_attackSensor, &_defenceSensor, &_spellSensor
	};

	String ReadCurrentMonsterPosition();
	String ReadCurrentSpellPosition();
	void SelectMultiplexerAddress(uint8_t address);
	int GetCardID(String cardID);

public:
	byte ZoneNumber;

	SingleCardZone monsterZone;
	SingleCardZone spellZone;

	DualCardZone(byte zoneNum, String monsterZoneName, String spellZoneName);
	void Initialize(PN532& reader);

	bool CheckForCardPresenceChanges();
	void UpdateCurrentMonster();
	void UpdateCurrentSpell();

	bool ScanForNewCard();
	bool ReadAvailableCard();
	void StopScanning();
	int GetCardSerialNumber();
	bool WriteRFIDTag(String cardID);
};

class MultiCardZone : public Zone {
public:
	MultiCardZone(String name);
};

#endif