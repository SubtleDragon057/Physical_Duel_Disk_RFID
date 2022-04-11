#include "Zone.h"
#include "Wire.h"

//#define DEBUG_DCZ
#define DEBUG_Readers

DualCardZone::DualCardZone(byte zoneNum) {	
	ZoneNumber = zoneNum;

	byte attackSensorAddress[4] = { 1,0,1,1 };
	byte defenceSensorAddress[4] = { 1,1,1,0 };
	byte spellSensorAddress[4] = { 0,1,0,1 };

	if (ZoneNumber == 1) {
		attackSensorAddress[0] = 0; attackSensorAddress[1] = 1;
		attackSensorAddress[2] = 1; attackSensorAddress[3] = 1;

		defenceSensorAddress[0] = 0; defenceSensorAddress[1] = 0;
		defenceSensorAddress[2] = 0; defenceSensorAddress[3] = 1;

		spellSensorAddress[0] = 1; spellSensorAddress[1] = 1;
		spellSensorAddress[2] = 0; spellSensorAddress[3] = 1;
	}
	else if (ZoneNumber == 2) {
		attackSensorAddress[0] = 1; attackSensorAddress[1] = 1;
		attackSensorAddress[2] = 1; attackSensorAddress[3] = 1;

		defenceSensorAddress[0] = 1; defenceSensorAddress[1] = 0;
		defenceSensorAddress[2] = 0; defenceSensorAddress[3] = 1;

		spellSensorAddress[0] = 0; spellSensorAddress[1] = 0;
		spellSensorAddress[2] = 1; spellSensorAddress[3] = 1;
	}

	_sensors[0]->UpdateAddress(attackSensorAddress);
	_sensors[1]->UpdateAddress(defenceSensorAddress);
	_sensors[2]->UpdateAddress(spellSensorAddress);
}

void DualCardZone::Initialize(PN532 &reader) {
	Serial.print(F("[BOOT] Initialize Zone "));	Serial.println(ZoneNumber);
	
	_reader = reader;

	SelectMultiplexerAddress(ZoneNumber);

	_reader.begin();
	_reader.performRFTest();
	_reader.setPassiveActivationRetries(10);

#ifdef DEBUG_Readers
	Serial.print(F("[DEBUG] "));
	uint32_t versiondata = reader.getFirmwareVersion();
	if (!versiondata) {
		Serial.print("Didn't find PN53x board on Zone ");
		Serial.println(ZoneNumber);
	}
	Serial.print("Found chip PN5"); Serial.println((versiondata >> 24) & 0xFF, HEX);
	Serial.print("Firmware ver. "); Serial.print((versiondata >> 16) & 0xFF, DEC);
	Serial.print('.'); Serial.println((versiondata >> 8) & 0xFF, DEC);
#endif

	for (byte i = 0; i < 3; i++) {
		_sensors[i]->Debug();
	}
}

bool DualCardZone::CheckForCardPresenceChanges() {
	monsterZoneCard.isCardPresent = _sensors[ProximitySensor::Attack]->isCardPresent();
	monsterZoneCard.hasCardPresenceChanged =
		monsterZoneCard.hasCardPresenceChanged != monsterZoneCard.isCardPresent;

	spellZoneCard.isCardPresent = _sensors[ProximitySensor::Spell]->isCardPresent();
	spellZoneCard.hasCardPresenceChanged =
		spellZoneCard.hasCardPresenceChanged != spellZoneCard.isCardPresent;

	return monsterZoneCard.hasCardPresenceChanged || spellZoneCard.hasCardPresenceChanged;
}

void DualCardZone::UpdateCurrentMonster() {
	if (!monsterZoneCard.isCardPresent) {
		monsterZoneCard.cardPosition = Enums::NoCard;
		return;
	}

	SelectMultiplexerAddress(ZoneNumber);
	
	bool error = ScanForNewCard();
	if (error) {
		//Handle error
		return;
	}

	// TODO: Ensure it gets proper NFC tag info
	monsterZoneCard.serialNum = GetCardSerialNumber();
	monsterZoneCard.cardPosition = ReadCurrentMonsterPosition();
}

void DualCardZone::UpdateCurrentSpell() {
	if (!spellZoneCard.isCardPresent) {
		spellZoneCard.cardPosition = Enums::NoCard;
		return;
	}

	SelectMultiplexerAddress(ZoneNumber);

	bool error = ScanForNewCard();
	if (error) {
		//Handle error
		return;
	}

	// TODO: Ensure it gets proper NFC tag info
	monsterZoneCard.serialNum = GetCardSerialNumber();
	monsterZoneCard.cardPosition = ReadCurrentSpellPosition();
}

bool DualCardZone::ScanForNewCard() {
	return _reader.readPassiveTargetID(PN532_MIFARE_ISO14443A, _uid, &_uidLength);;
}

// TODO: Add UID scan feature like MFRC library
bool DualCardZone::ReadAvailableCard() {
	return true;
}

// TODO: Implement HALT feature in PN532 Library
void DualCardZone::StopScanning() {
	
}

// TODO: Optimize ie. Remove use of String
String DualCardZone::GetCardSerialNumber() {	
	uint8_t readBackBlock[16];
	uint8_t keya[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

	uint8_t status = _reader.mifareclassic_AuthenticateBlock(_uid, _uidLength, _block, 0, keya);
	if (!status) {
		return "Failure";
	}

	status = _reader.mifareclassic_ReadDataBlock(_block, readBackBlock);
	if (!status) {
		Serial.print("[WARN] Could not read block!\n");
		return "Failure";
	}

#ifdef DEBUG_DCZ
	Serial.print("Reading Block 4:");
	Serial.println((char*)&readBackBlock);
#endif // DEBUG
	
	String cardSerial = (char*)&readBackBlock;
	return cardSerial;
}

bool DualCardZone::WriteRFIDTag(String cardID) {
	Serial.print("Writing ID: "); Serial.println(cardID);
	
	uint8_t keya[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
	uint8_t status = _reader.mifareclassic_AuthenticateBlock(_uid, _uidLength, _block, 0, keya);
	Serial.print("Authenticate Write: "); Serial.println(status);
	if (status != 1) return false;

	uint8_t serial;
	for (byte i = 0; i < 9; i++) {
		serial += cardID[i];
	}

	Serial.print("Atempting ID: "); Serial.println(serial);
	
	status = _reader.mifareclassic_WriteDataBlock(_block, &serial);
	Serial.print("Write Success: "); Serial.println(status);
	return status;
}

Enums::CardPosition DualCardZone::ReadCurrentMonsterPosition() {
	bool hasDefCard = _sensors[ProximitySensor::Defence]->isCardPresent();
	if (!hasDefCard) {
		return Enums::FaceUp;
	}

	switch (_sensors[ProximitySensor::Defence]->CurrentValue) {
	case Enums::Low:
		return Enums::FaceDownDefence;
		break;
	case Enums::Medium:
		return Enums::FaceUpDefence;
		break;
	default:
		return Enums::NoCard;
	}
}

Enums::CardPosition DualCardZone::ReadCurrentSpellPosition() {
	switch (_sensors[ProximitySensor::Spell]->CurrentValue) {
		case Enums::Low:
			return Enums::FaceDown;
			break;
		case Enums::Medium:
			return Enums::FaceUp;
			break;
		default:
			return Enums::NoCard;
	}
}

void DualCardZone::SelectMultiplexerAddress(uint8_t address) {
	if (address > 7) return;

	Wire.beginTransmission(_multiplexerAddress);
	Wire.write(1 << address);
	Wire.endTransmission();
}