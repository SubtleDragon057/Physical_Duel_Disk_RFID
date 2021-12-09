#include "Zone.h"
//#include "Entities\Components\AnalogIR.h"
//#include "Entities\Components\DigitalIR.h"

DualCardZone::DualCardZone(bool isMFRC)
{
	_isMFRC = isMFRC;
}

void DualCardZone::Initialize(int zoneNum, byte readerPin, byte resetPin, byte blockNumber, byte attackSensorPin,
	byte defenceSensorPin, byte spellSensorPin)
{
	if (_isMFRC)
		_mfrc = mfrcReader(readerPin, resetPin, blockNumber);
	else
		_pn532 = PN532Reader(readerPin, blockNumber);
	
	ZoneNumber = zoneNum;
	AttackSensor = DigitalIR(attackSensorPin);
	DefenceSensor = AnalogIR(defenceSensorPin, false);
	SpellSensor = AnalogIR(spellSensorPin, false);

	if (_debug) {
		Serial.print(F("[DEBUG] "));
		if (_isMFRC)
			_mfrc.DebugReader();
		else
			_pn532.DebugReader();
	}

	_currentMonster = Monster("", Enums::NoCard);
	_currentSpell = Spell("", Enums::NoCard);
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
	_currentMonster.UpdateCard(monsterID, position);
}

void DualCardZone::UpdateCurrentSpell(String spellID, Enums::CardPosition position) {
	_currentSpell.UpdateCard(spellID, position);
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
	return _isMFRC ? _mfrc.ScanForNewCard() : _pn532.ScanForNewCard();
}

bool DualCardZone::ReadAvailableCard()
{
	return _isMFRC ? _mfrc.ReadAvailableCard() : _pn532.ReadAvailableCard();
}

void DualCardZone::StopScanning()
{
	_isMFRC ? _mfrc.StopScanning() : _pn532.StopScanning();
}

String DualCardZone::GetCardSerialNumber(byte readBackBlock[]) {

	// Clear Previous Card Value
	memset(readBackBlock, 0, sizeof(readBackBlock));
	
	_isMFRC ? _mfrc.ReadBlock(readBackBlock) : _pn532.ReadBlock(readBackBlock);

	String cardSerial = (String((char*)readBackBlock)).substring(1, 16);
	return cardSerial;
}
