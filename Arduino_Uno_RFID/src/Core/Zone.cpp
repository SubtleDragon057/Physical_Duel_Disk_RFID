#include "Zone.h"

DualCardZone::DualCardZone()
{
}

void DualCardZone::Initialize(byte zoneNum, PN532 &reader, byte attackSensorAddress[],
	byte defenceSensorAddress[], byte spellSensorAddress[]) {
	Serial.print("[BOOT] Initialize Zone ");
	Serial.println(zoneNum);
	
	_reader = reader;
	ZoneNumber = zoneNum;
	_sensors[0] = AnalogIR(attackSensorAddress, 1, _debug);
	_sensors[1] = AnalogIR(defenceSensorAddress, 2, _debug);
	_sensors[2] = AnalogIR(spellSensorAddress, 3, _debug);
}

Enums::CardPosition DualCardZone::ReadCurrentMonsterPosition() {
	Enums::CardPosition position = Enums::NoCard;
	int lightValue = _sensors[1].CurrentValue;

	switch (lightValue) {
		case Enums::Low:
			position = Enums::FaceDownDefence;
			break;
		case Enums::Medium:
			position = Enums::FaceUpDefence;
			break;
	}
	
	if (_sensors[0].CurrentValue == Enums::Low) {
		position = Enums::FaceUp;
	}

	return position;
}

Enums::CardPosition DualCardZone::ReadCurrentSpellPosition() {
	Enums::CardPosition position = Enums::NoCard;
	int lightValue = _sensors[2].CurrentValue;

	switch (lightValue) {
		case Enums::Low:
			position = Enums::FaceDownDefence;
			break;
		case Enums::Medium:
			position = Enums::FaceUpDefence;
			break;
	}

	return position;
}

void DualCardZone::UpdateCurrentMonster(String monsterID, Enums::CardPosition position) {
	MonsterSerial = monsterID;
	MonsterPosition = position;
}

void DualCardZone::UpdateCurrentSpell(String spellID, Enums::CardPosition position) {
	SpellSerial = spellID;
	SpellPosition = position;
}

void DualCardZone::CheckForTrippedSensors() {

	// TODO: Is this delay needed?
	delay(250);
	
	for (byte i = 0; i < 3; i++) {
		//Serial.print("Sensor "); Serial.print(i); Serial.print(" at ");
		if (!_sensors[i].isNewCardPresent()) continue;

		if (_debug) {
			Serial.print("Sensor Tripped: "); Serial.println(i);
		}
		
		TrippedSensors[i] = true;
	}
}

bool DualCardZone::ScanForNewCard()
{
	return _reader.readPassiveTargetID(PN532_MIFARE_ISO14443A, _uid, &_uidLength);
}

// TODO: Add UID scan feature like MFRC library
bool DualCardZone::ReadAvailableCard()
{
	return true;
}

// TODO: Implement HALT feature in PN532 Library
void DualCardZone::StopScanning()
{
	
}

String DualCardZone::GetCardSerialNumber() {	
	uint8_t readBackBlock[16];
	uint8_t keya[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

	uint8_t status = _reader.mifareclassic_AuthenticateBlock(_uid, _uidLength, _block, 0, keya);
	if (!status) {
		return "3";
	}

	status = _reader.mifareclassic_ReadDataBlock(_block, readBackBlock);
	if (!status) {
		Serial.print("[WARN] Could not read block!\n");
		return "4";
	}

	if (_debug) {
		Serial.print("Reading Block 4:");
		Serial.println((char*)&readBackBlock);
	}
	
	String cardSerial = (char*)&readBackBlock;
	return cardSerial.substring(1);
}