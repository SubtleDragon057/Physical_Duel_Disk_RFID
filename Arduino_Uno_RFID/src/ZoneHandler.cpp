#include "ZoneHandler.h"

ZoneHandler::ZoneHandler(bool debug) 
{
	_debug = debug;
}

void ZoneHandler::Initialize(byte numZones, byte readerPins[], byte attackSensorPins[], 
	byte defenceSensorPins[], byte spellSensorPins[]) {
	
	for (int i = 0; i < numZones; i++) {
		Zones[i].Initialize(i,
			readerPins[i], 
			readerPins[3],
			_block,
			attackSensorPins[i],
			defenceSensorPins[i],
			spellSensorPins[i]);

		delay(10);
	}
}

Enums::SensorType ZoneHandler::CheckForTrippedSensor(int zoneNumber) {
	
	Enums::SensorType isNewCardPresent = Zones[zoneNumber].isNewCardPresent();
	if (isNewCardPresent != Enums::None) {		
		delay(100); // delay to ensure card is fully placed
		CheckRFIDReader(Zones[zoneNumber], isNewCardPresent);
	}

	return isNewCardPresent;
}

void ZoneHandler::CheckRFIDReader(DualCardZone &zone, Enums::SensorType sensor) {
	bool isMonster = false;
	Enums::Events eventType = Enums::NoEvent;
	
	bool hasNewCard = zone.ScanForNewCard();

	if (!hasNewCard) {
		HandleUpdateCard(zone, sensor, true);		
		zone.StopScanning();
		return;
	}

	// Scan Readers twice in case of collision (ie. Two cards)
	for (int i = 0; i < 2; i++) {

		bool hasAvailableCard = zone.ReadAvailableCard();
		if (!hasAvailableCard) continue;

		HandleUpdateCard(zone, sensor);
	}

	zone.StopScanning();
}

void ZoneHandler::HandleUpdateCard(DualCardZone& zone, Enums::SensorType sensor, bool isRemoval = false) {
	String cardSerialNumber = zone.GetCardSerialNumber(_readBackBlock);
	Enums::CardPosition position;

	if (sensor == Enums::SpellTrap) {
		position = zone.ReadCurrentSpellPosition();
		zone.UpdateCurrentSpell(cardSerialNumber, position);
		return; // TODO: Handle Spell Removal
	}

	if (isRemoval) {
		zone.UpdateCurrentMonster(zone.GetCurrentMonster().GetSerialNumber(), Enums::NoCard);
		return;
	}

	position = zone.ReadCurrentMonsterPosition();
	zone.UpdateCurrentMonster(cardSerialNumber, position);
}