#include "Arduino.h"
#include "Zone.h"
#include "MFRC522.h"
#include "Entities\Enums.h"

DualCardZone::DualCardZone()
{
}

void DualCardZone::Initialize(int zoneNum, MFRC522 reader, ProximitySensor attackSensor, 
	AnalogIR defenceSensor, AnalogIR spellSensor) 
{
	ZoneNumber = zoneNum;
	Reader = reader;
	AttackSensor = attackSensor;
	DefenceSensor = defenceSensor;
	SpellSensor = spellSensor;

	_currentMonster = Monster("", Enums::NoCard);
	_currentSpell = Spell("", Enums::NoCard);
}

Monster DualCardZone::GetCurrentMonster() {
	return _currentMonster;
}

Spell DualCardZone::GetCurrentSpell() {
	return _currentSpell;
}

void DualCardZone::UpdateCurrentMonster(String monsterID, Enums::CardPosition position) {
	_currentMonster.UpdateCard(monsterID, position);
}

void DualCardZone::UpdateCurrentSpell(String spellID, Enums::CardPosition position) {
	_currentSpell.UpdateCard(spellID, position);
}

Enums::SensorType DualCardZone::isNewCardPresent() {

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
	Reader.PCD_Init();
	return Reader.PICC_IsNewCardPresent();
}

bool DualCardZone::ReadAvailableCard()
{
	return Reader.PICC_ReadCardSerial();
}

void DualCardZone::StopScanning()
{
	Reader.PICC_HaltA();
	Reader.PCD_StopCrypto1();
}
