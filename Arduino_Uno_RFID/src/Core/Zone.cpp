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
	_sensors[0] = AnalogIR(attackSensorAddress, true, _debug);
	_sensors[1] = AnalogIR(defenceSensorAddress, false, _debug);
	_sensors[2] = AnalogIR(spellSensorAddress, false, _debug);
}

Enums::CardPosition DualCardZone::ReadCurrentMonsterPosition()
{
	int lightValue = _sensors[1].GetCurrentValue();

	if (lightValue == Enums::Low) {
		return Enums::FaceDownDefence;
	}
	else if (lightValue == Enums::Medium) {
		return Enums::FaceUpDefence;
	}
	else if (_sensors[0].GetCurrentValue() == Enums::Low) {
		return Enums::FaceUp;
	}

	return Enums::NoCard;
}

Enums::CardPosition DualCardZone::ReadCurrentSpellPosition()
{
	int lightValue = _sensors[2].GetCurrentValue();

	if (lightValue == Enums::Low) {
		return Enums::FaceUp;
	}
	else if (lightValue == Enums::Medium) {
		return Enums::FaceDown;
	}

	return Enums::NoCard;
}

void DualCardZone::UpdateCurrentMonster(String monsterID, Enums::CardPosition position) {
	MonsterSerial = monsterID;
	MonsterPosition = position;
}

void DualCardZone::UpdateCurrentSpell(String spellID, Enums::CardPosition position) {
	SpellSerial = spellID;
	SpellPosition = position;
}

int DualCardZone::isNewCardPresent() {

	// TODO: Is this delay needed?
	delay(250);
	
	byte sensor = Enums::None;
	for (byte i = 0; i < 3; i++) {
		//Serial.print("Sensor "); Serial.print(i); Serial.print(" at ");
		if (!_sensors[i].isNewCardPresent()) continue;

		sensor = i;
	}

	return sensor;
}

bool DualCardZone::ScanForNewCard()
{
	return _reader.readPassiveTargetID(PN532_MIFARE_ISO14443A, _uid, &_uidLength);
}

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