#ifndef _ZONE_h
#define _ZONE_h
#include "Arduino.h"
#include "Entities\Enums.h"
#include "Entities\YgoCard.h"
#include "Entities\Components.h"

class DualCardZone {
private:
	Monster _currentMonster;
	Spell _currentSpell;

	bool _debug = true;

public:
	int ZoneNumber;
	nfcReader* Reader;
	ProximitySensor AttackSensor;
	AnalogIR DefenceSensor;
	AnalogIR SpellSensor;

	DualCardZone();
	void Initialize(int zoneNum, byte readerPin, byte resetPin, ProximitySensor attackSensor, 
		AnalogIR defenceSensor, AnalogIR spellSensor);

	Monster GetCurrentMonster() { return _currentMonster; }
	Spell GetCurrentSpell() { return _currentSpell; }
	void UpdateCurrentMonster(String monsterID, Enums::CardPosition position);
	void UpdateCurrentSpell(String spellID, Enums::CardPosition position);

	Enums::SensorType isNewCardPresent();

	bool ScanForNewCard();
	bool ReadAvailableCard();
	void StopScanning();
	String GetCardSerialNumber(byte readBackBlock[]);
};

#endif