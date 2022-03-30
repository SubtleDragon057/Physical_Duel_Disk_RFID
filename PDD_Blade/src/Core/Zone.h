#ifndef _ZONE_h
#define _ZONE_h
#include "Arduino.h"
#include "Entities\Enums.h"
#include "Entities\Components\ProximitySensor.h"
#include "PN532.h"

class DualCardZone {
private:
	PN532 _reader;
	byte _block = 4;
	byte _uid[7];
	byte _uidLength;

	const uint8_t _multiplexerAddress = 0x70;

	ProximitySensor _sensors[3] = {
		ProximitySensor(ProximitySensor::Attack),
		ProximitySensor(ProximitySensor::Defence),
		ProximitySensor(ProximitySensor::Spell)
	};

	struct YGOCard {
		bool isCardPresent = false;
		bool hasCardPresenceChanged = false;
		String serialNum = "";
		Enums::CardPosition cardPosition = Enums::NoCard;
	};

	Enums::CardPosition ReadCurrentMonsterPosition();
	Enums::CardPosition ReadCurrentSpellPosition();
	void SelectMultiplexerAddress(uint8_t address);

public:	
	byte ZoneNumber;	
	
	YGOCard monsterZoneCard;
	YGOCard spellZoneCard;

	DualCardZone(byte zoneNum);
	void Initialize(PN532 &reader);

	bool CheckForCardPresenceChanges();
	void UpdateCurrentMonster();
	void UpdateCurrentSpell();

	bool ScanForNewCard();
	bool ReadAvailableCard();
	void StopScanning();
	String GetCardSerialNumber();
	bool WriteRFIDTag(String cardID);
};

#endif