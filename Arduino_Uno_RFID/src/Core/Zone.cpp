#include "Zone.h"

DualCardZone::DualCardZone()
{
}

void DualCardZone::Initialize(int zoneNum, PN532 &reader, byte blockNumber, byte attackSensorPin,
	byte defenceSensorPin, byte spellSensorPin) {
	Serial.print("[BOOT] Initialize Zone ");
	Serial.println(zoneNum);
	
	_pn532 = reader;
	
	_block = blockNumber;
	ZoneNumber = zoneNum;
	AttackSensor = DigitalIR(attackSensorPin);
	DefenceSensor = AnalogIR(defenceSensorPin, false);
	SpellSensor = AnalogIR(spellSensorPin, false);

	_currentMonster = &Monster("", Enums::NoCard);
	_currentSpell = &Spell("", Enums::NoCard);
}

Enums::CardPosition DualCardZone::ReadCurrentMonsterPosition()
{
	int lightValue = DefenceSensor.GetCurrentValue();

	if (lightValue == Enums::Low) {
		return Enums::FaceDownDefence;
	}
	else if (lightValue == Enums::Medium) {
		return Enums::FaceUpDefence;
	}
	else if (AttackSensor.GetCurrentValue() == Enums::Low) {
		return Enums::FaceUp;
	}

	return Enums::NoCard;
}

Enums::CardPosition DualCardZone::ReadCurrentSpellPosition()
{
	int lightValue = SpellSensor.GetCurrentValue();

	if (lightValue == Enums::Low) {
		return Enums::FaceUp;
	}
	else if (lightValue == Enums::Medium) {
		return Enums::FaceDown;
	}

	return Enums::NoCard;
}

void DualCardZone::UpdateCurrentMonster(String monsterID, Enums::CardPosition position) {
	_currentMonster->UpdateCard(monsterID, position);
}

void DualCardZone::UpdateCurrentSpell(String spellID, Enums::CardPosition position) {
	_currentSpell->UpdateCard(spellID, position);
}

Enums::SensorType DualCardZone::isNewCardPresent() {

	// TODO: Is this delay needed?
	delay(250);
	
	if (AttackSensor.isNewCardPresent()) {
		return Enums::MonsterAttack;
	}
	else if (DefenceSensor.isNewCardPresent()) {
		return Enums::MonsterDefence;
	}
	/*else if (SpellSensor.isNewCardPresent()) {
		return Enums::SpellTrap;
	}*/

	return Enums::None;
}

bool DualCardZone::ScanForNewCard()
{
	return _pn532.readPassiveTargetID(PN532_MIFARE_ISO14443A, _uid, &_uidLength);
}

bool DualCardZone::ReadAvailableCard()
{
	return true;
}

void DualCardZone::StopScanning()
{
	
}

String DualCardZone::GetCardSerialNumber() {	
	String cardSerial = PN532ReadBlock();
	return cardSerial.substring(1);
}

String DualCardZone::PN532ReadBlock()
{
	uint8_t readBackBlock[16];
	int trailerBlock = (_block / 4 * 4) + 3;
	byte buffersize = 18;

	uint8_t keya[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
	uint8_t status = _pn532.mifareclassic_AuthenticateBlock(_uid, _uidLength, _block, 0, keya);
	if (!status) {
		return "3";
	}

	status = _pn532.mifareclassic_ReadDataBlock(_block, readBackBlock);
	if (!status) {
		Serial.print("[WARN] Could not read block!\n");
		return "4";
	}

	Serial.print("Reading Block 4:");
	Serial.println((char*)&readBackBlock);

	return (char*)&readBackBlock;
}
