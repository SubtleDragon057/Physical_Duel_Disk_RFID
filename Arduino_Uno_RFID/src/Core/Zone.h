#ifndef _ZONE_h
#define _ZONE_h
#include "Arduino.h"
#include "Entities\Enums.h"
#include "Entities\YgoCard.h"
#include "Entities\Components\MFRC522Reader.h"
#include "Entities\Components\PN532Reader.h"
//#include "Entities\Components\Models\ProximitySensor.h"
#include "Entities\Components\AnalogIR.h"
#include "Entities\Components\DigitalIR.h"

class DualCardZone {
private:
	Monster _currentMonster;
	Spell _currentSpell;

	bool _isMFRC;
	mfrcReader _mfrc;
	PN532Reader _pn532;

	bool _debug = true;

public:
	int ZoneNumber;
	DigitalIR AttackSensor;
	AnalogIR DefenceSensor;
	AnalogIR SpellSensor;

	DualCardZone(bool isMFRC);
	void Initialize(int zoneNum, byte readerPin, byte resetPin, byte blockNumber, byte attackSensorPin,
		byte defenceSensorPin, byte spellSensorPin);

	Monster GetCurrentMonster() { return _currentMonster; }
	Spell GetCurrentSpell() { return _currentSpell; }
	Enums::CardPosition ReadCurrentMonsterPosition();
	Enums::CardPosition ReadCurrentSpellPosition();
	void UpdateCurrentMonster(String monsterID, Enums::CardPosition position);
	void UpdateCurrentSpell(String spellID, Enums::CardPosition position);

	Enums::SensorType isNewCardPresent();

	bool ScanForNewCard();
	bool ReadAvailableCard();
	void StopScanning();
	String GetCardSerialNumber(byte readBackBlock[]);
};

#endif