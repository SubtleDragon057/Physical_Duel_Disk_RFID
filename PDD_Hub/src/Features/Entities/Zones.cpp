#include "Zones.h"
#include "Wire.h"

//#define DEBUG_DCZ
//#define DEBUG_Readers

Zone::Zone(String zoneName) {
	ZoneName = zoneName;
}

SingleCardZone::SingleCardZone(String zoneName) {
	ZoneName = zoneName;
}

void SingleCardZone::UpdateCurrentCard(int serialNum, int copyNum, String position) {
	Card.serialNum = serialNum;
	Card.copyNum = copyNum;
	Card.cardPosition = position;
}

// TODO: FInd a more efficient way to run this
DualCardZone::DualCardZone(byte zoneNum, String monsterZoneName, String spellZoneName) {
	ZoneNumber = zoneNum;
	monsterZone.ZoneName = monsterZoneName;
	spellZone.ZoneName = spellZoneName;

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

void DualCardZone::Initialize(PN532& reader) {
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
}

bool DualCardZone::CheckForCardPresenceChanges() {
	monsterZone.Card.isCardPresent = _sensors[ProximitySensor::Attack]->isCardPresent();
	monsterZone.Card.hasCardPresenceChanged =
		monsterZone.Card.hasCardPresenceChanged != monsterZone.Card.isCardPresent;

	spellZone.Card.isCardPresent = _sensors[ProximitySensor::Spell]->isCardPresent();
	spellZone.Card.hasCardPresenceChanged =
		spellZone.Card.hasCardPresenceChanged != spellZone.Card.isCardPresent;

	return monsterZone.Card.hasCardPresenceChanged || spellZone.Card.hasCardPresenceChanged;
}

void DualCardZone::UpdateCurrentMonster() {
	if (!monsterZone.Card.isCardPresent) {
		monsterZone.Card.cardPosition = "";
		return;
	}

	SelectMultiplexerAddress(ZoneNumber);

	bool error = ScanForNewCard();
	if (error) {
		// TODO: Handle NFC error
		return;
	}

	// TODO: Ensure it gets proper NFC tag info
	// TODO: Get Copy Number
	monsterZone.Card.serialNum = GetCardSerialNumber();
	monsterZone.Card.cardPosition = ReadCurrentMonsterPosition();
}

void DualCardZone::UpdateCurrentSpell() {
	if (!spellZone.Card.isCardPresent) {
		spellZone.Card.cardPosition = "";
		return;
	}

	SelectMultiplexerAddress(ZoneNumber);

	bool error = ScanForNewCard();
	if (error) {
		//Handle error
		return;
	}

	// TODO: Ensure it gets proper NFC tag info
	// TODO: Get Copy Number
	spellZone.Card.serialNum = GetCardSerialNumber();
	spellZone.Card.cardPosition = ReadCurrentSpellPosition();
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
int DualCardZone::GetCardSerialNumber() {
	uint8_t readBackBlock[16];
	uint8_t keya[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

	uint8_t status = _reader.mifareclassic_AuthenticateBlock(_uid, _uidLength, _block, 0, keya);
	if (!status) {
		return (int)status;
	}

	status = _reader.mifareclassic_ReadDataBlock(_block, readBackBlock);
	if (!status) {
		Serial.print("[WARN] Could not read block!\n");
		return (int)status;
	}

#ifdef DEBUG_DCZ
	Serial.print("Reading Block 4:");
	Serial.println((char*)&readBackBlock);
#endif // DEBUG

	String cardSerial = (char*)&readBackBlock;
	return GetCardID(cardSerial.substring(1));
}

int DualCardZone::GetCardID(String cardID) {
	char charArray[9];
	cardID.toCharArray(charArray, 9);

	return atoi(&charArray[0]);
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

String DualCardZone::ReadCurrentMonsterPosition() {
	bool hasDefCard = _sensors[ProximitySensor::Defence]->isCardPresent();
	if (!hasDefCard) {
		return "FaceUp";
	}

	switch (_sensors[ProximitySensor::Defence]->CurrentValue) {
	case ProximitySensor::Low:
		return "FaceDownDefence";
		break;
	case ProximitySensor::Medium:
		return "FaceUpDefence";
		break;
	default:
		return "";
	}
}

String DualCardZone::ReadCurrentSpellPosition() {
	switch (_sensors[ProximitySensor::Spell]->CurrentValue) {
	case ProximitySensor::Low:
		return "FaceDown";
		break;
	case ProximitySensor::Medium:
		return "FaceUp";
		break;
	default:
		return "NoCard";
	}
}

// TODO: Use Peripherals Handler for this
void DualCardZone::SelectMultiplexerAddress(uint8_t address) {
	if (address > 7) return;

	Wire.beginTransmission(_multiplexerAddress);
	Wire.write(1 << address);
	Wire.endTransmission();
}

MultiCardZone::MultiCardZone(String zoneName) {
	ZoneName = zoneName;
}