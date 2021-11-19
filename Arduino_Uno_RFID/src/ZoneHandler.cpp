#include "Arduino.h"
#include "ZoneHandler.h"
#include "Core\Entities\Enums.h"
#include "Core\Entities\YgoCard.h"
#include "Core\Zone.h"
#include "Features\RFIDFunctions\RFIDFunctions.h"

ZoneHandler::ZoneHandler(bool debug = false) 
{
	_debug = debug;
}

void ZoneHandler::Initialize(byte numZones, byte readerPins[], ProximitySensor attackSensors[], 
	AnalogIR defenceSensors[], AnalogIR spellSensors[]) {
	
	for (int i = 0; i < numZones; i++) {
		_cardSlots[i].PCD_Init(readerPins[i], readerPins[4]);
		_cardSlots[i].PCD_SetAntennaGain(MFRC522::PCD_RxGain::RxGain_avg);

		Zones[i].Initialize(i,
			_cardSlots[i],
			attackSensors[i],
			defenceSensors[i],
			spellSensors[i]);

		delay(10);
	}

	rfidFunctions.Initialize(_block, _cardsToRead);
}
void ZoneHandler::Initialize(byte numZones, byte readerPins[], ProximitySensor attackSensors[],
	AnalogIR defenceSensors[], AnalogIR spellSensors[], byte customKey[]) {

	for (int i = 0; i < numZones; i++) {
		_cardSlots[i].PCD_Init(readerPins[i], readerPins[4]);
		_cardSlots[i].PCD_SetAntennaGain(MFRC522::PCD_RxGain::RxGain_avg);

		Zones[i].Initialize(i,
			_cardSlots[i],
			attackSensors[i],
			defenceSensors[i],
			spellSensors[i]);

		delay(10);
	}

	rfidFunctions.Initialize(_block, customKey, _cardsToRead);
}

Enums::SensorType ZoneHandler::CheckZone(int zoneNumber) {
	Enums::SensorType isNewCardPresent = Zones[zoneNumber].isNewCardPresent();

	if (isNewCardPresent != Enums::None) {
		if (_debug) {
			Serial.print(zoneNumber);
			Serial.print(": Tripped Sensor: ");
			Serial.println(isNewCardPresent);
		}
		
		delay(100);
		CheckRFIDReader(Zones[zoneNumber], isNewCardPresent);
	}

	return isNewCardPresent;
}

void ZoneHandler::CheckRFIDReader(DualCardZone &zone, Enums::SensorType sensor) {
	bool isMonster = false;
	Enums::Events eventType = Enums::NoEvent;

	bool hasNewCard = zone.ScanForNewCard();

	if (!hasNewCard) {
		HandleUpdateCard(zone, sensor);		
		zone.StopScanning();
		return;
	}

	// Scan Readers twice in case of collision (ie. Two cards)
	for (int i = 0; i < 2; i++) {

		bool hasAvailableCard = zone.ReadAvailableCard();
		if (!hasAvailableCard) {
			continue;
		}

		HandleUpdateCard(zone, sensor);
	}

	zone.StopScanning();
}

Enums::CardPosition ZoneHandler::GetMonsterPosition(ProximitySensor attackSensor, AnalogIR defenceSensor) {	
	int lightValue = defenceSensor.GetCurrentValue();

	if (lightValue == Enums::Low) {
		return Enums::FaceDownDefence;
	}
	else if (lightValue == Enums::Medium) {
		return Enums::FaceUpDefence;
	}
	else if(attackSensor.GetCurrentValue() == Enums::Low) {
		return Enums::FaceUp;
	}

	return Enums::NoCard;
}

Enums::CardPosition ZoneHandler::GetSpellPosition(AnalogIR spellSensor) {
	int lightValue = spellSensor.GetCurrentValue();

	if (lightValue == Enums::Low) {
		return Enums::FaceUp;
	}
	else if (lightValue == Enums::Medium) {
		return Enums::FaceDown;
	}

	return Enums::NoCard;
}

void ZoneHandler::HandleUpdateCard(DualCardZone& zone, Enums::SensorType sensor) {
	String cardSerialNumber = GetCardSerialNumber(zone.Reader);
	Enums::CardPosition position;

	if (sensor == Enums::SpellTrap) {
		position = GetSpellPosition(zone.SpellSensor);
		zone.UpdateCurrentSpell(cardSerialNumber, position);
	}

	position = GetMonsterPosition(zone.AttackSensor, zone.DefenceSensor);
	zone.UpdateCurrentMonster(cardSerialNumber, position);
}

String ZoneHandler::GetCardSerialNumber(MFRC522 reader)
{
	rfidFunctions.ReadBlock(reader, _readBackBlock);
	return (String((char*)_readBackBlock)).substring(1, 16);
}