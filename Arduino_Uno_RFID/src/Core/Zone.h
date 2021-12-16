#ifndef _ZONE_h
#define _ZONE_h
#include "Arduino.h"
#include "Entities\Enums.h"
#include "Entities\Components\AnalogIR.h"
#include "PN532.h"

class DualCardZone {
private:
	PN532 _reader;
	byte _block = 4;
	byte _uid[7];
	byte _uidLength;

	AnalogIR _sensors[3] = {};

	bool _debug = true;

public:
	int ZoneNumber;
	bool TrippedSensors[3] = { false, false, false };

	String MonsterSerial = "";
	Enums::CardPosition MonsterPosition = Enums::NoCard;

	String SpellSerial = "";
	Enums::CardPosition SpellPosition = Enums::NoCard;

	DualCardZone();
	void Initialize(byte zoneNum, PN532 &reader, byte attackSensorAddress[],
		byte defenceSensorAddress[], byte spellSensorAddress[]);

	Enums::CardPosition ReadCurrentMonsterPosition();
	Enums::CardPosition ReadCurrentSpellPosition();
	void UpdateCurrentMonster(String monsterID, Enums::CardPosition position);
	void UpdateCurrentSpell(String spellID, Enums::CardPosition position);

	void CheckForTrippedSensors();

	bool ScanForNewCard();
	bool ReadAvailableCard();
	void StopScanning();
	String GetCardSerialNumber();
};

#endif