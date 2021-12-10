#ifndef _ZONE_h
#define _ZONE_h
#include "Arduino.h"
#include "Entities\Enums.h"
#include "Entities\YgoCard.h"
#include "Entities\Components\PN532Reader.h"
//#include "Entities\Components\Models\ProximitySensor.h"
#include "Entities\Components\AnalogIR.h"
#include "Entities\Components\DigitalIR.h"
#include "PN532.h"

class DualCardZone {
private:
	Monster * _currentMonster;
	Spell * _currentSpell;

	PN532 _pn532;

	byte _block;

	byte _uid[7];
	byte _uidLength;
	uint8_t _key[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

	bool _debug = true;

	String PN532ReadBlock();

public:
	int ZoneNumber;
	DigitalIR AttackSensor;
	AnalogIR DefenceSensor;
	AnalogIR SpellSensor;

	DualCardZone();
	void Initialize(int zoneNum, PN532 reader, byte blockNumber, byte attackSensorPin,
		byte defenceSensorPin, byte spellSensorPin);

	Monster GetCurrentMonster() { return *_currentMonster; }
	Spell GetCurrentSpell() { return *_currentSpell; }
	Enums::CardPosition ReadCurrentMonsterPosition();
	Enums::CardPosition ReadCurrentSpellPosition();
	void UpdateCurrentMonster(String monsterID, Enums::CardPosition position);
	void UpdateCurrentSpell(String spellID, Enums::CardPosition position);

	Enums::SensorType isNewCardPresent();

	bool ScanForNewCard();
	bool ReadAvailableCard();
	void StopScanning();
	String GetCardSerialNumber();
};

#endif