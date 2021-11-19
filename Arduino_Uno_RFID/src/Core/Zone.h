#ifndef _ZONE_h
#define _ZONE_h
#include "Arduino.h"
#include "MFRC522.h"
#include "Entities\Enums.h"
#include "Entities\YgoCard.h"
#include "Entities\Components.h"

class DualCardZone {
private:
	Monster _currentMonster;
	Spell _currentSpell;

public:
	int ZoneNumber;
	MFRC522 Reader;
	ProximitySensor AttackSensor;
	AnalogIR DefenceSensor;
	AnalogIR SpellSensor;

	DualCardZone();
	void Initialize(int zoneNum, MFRC522 reader, ProximitySensor attackSensor, 
		AnalogIR defenceSensor, AnalogIR spellSensor);

	Monster GetCurrentMonster();
	Spell GetCurrentSpell();
	void UpdateCurrentMonster(String monsterID, Enums::CardPosition position);
	void UpdateCurrentSpell(String spellID, Enums::CardPosition position);

	Enums::SensorType isNewCardPresent();

	bool ScanForNewCard();
	bool ReadAvailableCard();
	void StopScanning();
};

#endif