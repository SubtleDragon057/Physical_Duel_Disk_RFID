#include "Arduino.h"
#include "Zone.h"
#include "Entities\Enums.h"
#include "Entities\Components.h"

DualCardZone::DualCardZone()
{
}

void DualCardZone::Initialize(int zoneNum, byte readerPin, byte resetPin, ProximitySensor attackSensor,
	AnalogIR defenceSensor, AnalogIR spellSensor) 
{
	bool isMFRC = zoneNum != 2;
	if (isMFRC) {		
		Reader = &mfrcReader(readerPin, resetPin);
	}
	else {		
		Reader = &pnReader(readerPin, resetPin);
	}
	
	ZoneNumber = zoneNum;
	AttackSensor = attackSensor;
	DefenceSensor = defenceSensor;
	SpellSensor = spellSensor;

	if (_debug) {
		Serial.print(F("[DEBUG] "));
		Reader->DebugReader();
	}

	_currentMonster = Monster("", Enums::NoCard);
	_currentSpell = Spell("", Enums::NoCard);
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
	return Reader->ScanForNewCard();
}

bool DualCardZone::ReadAvailableCard()
{
	return Reader->ReadAvailableCard();
}

void DualCardZone::StopScanning()
{
	Reader->StopScanning();
}

String DualCardZone::GetCardSerialNumber(byte readBackBlock[]) {
	if (_debug) {
		Serial.print(F("[DEBUG] GetCardSerial Entered"));
	}
	
	Reader->ReadBlock(readBackBlock);

	if (_debug) {
		Serial.print(F("[DEBUG] GetCardSerial: "));
		Serial.println((String((char*)readBackBlock)).substring(1, 16));
	}

	return (String((char*)readBackBlock)).substring(1, 16);
}
