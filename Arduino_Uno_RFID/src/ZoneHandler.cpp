#include "ZoneHandler.h"
#include "Core\Entities\YgoCard.h"

ZoneHandler::ZoneHandler(CommunicationsHandler& communicationsHandler, bool debug)
{
	_communicationsHandler = communicationsHandler;
	_debug = debug;
}

void ZoneHandler::Initialize(byte numZones, byte attackSensorPins[], PN532 &reader,
	byte defenceSensorPins[], byte spellSensorPins[]) {
	Serial.println("[BOOT] Initialize Zone Hanlder");

	for (byte i = 0; i < numZones; i++) {
		_communicationsHandler.SelectMultiplexerAddress(i);
		Zones[i].Initialize(i,
			reader,
			_block,
			attackSensorPins[i],
			defenceSensorPins[i],
			spellSensorPins[i]);
	}

	delay(10);
}

Enums::SensorType ZoneHandler::CheckForTrippedSensor(int zoneNumber) {
	
	Enums::SensorType isNewCardPresent = Zones[zoneNumber].isNewCardPresent();
	if (isNewCardPresent != Enums::None) {		
		delay(100); // delay to ensure card is fully placed
		_communicationsHandler.SelectMultiplexerAddress(zoneNumber);
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
	String cardSerialNumber = zone.GetCardSerialNumber();
	Enums::CardPosition position;

	if (sensor == Enums::SpellTrap) {
		position = zone.ReadCurrentSpellPosition();
		zone.UpdateCurrentSpell(cardSerialNumber, position);
		return; // TODO: Handle Spell Removal
	}

	if (isRemoval) {
		Monster zoneMonster = zone.GetCurrentMonster();
		String monsterSerial = zoneMonster.GetSerialNumber();
		Serial.print("Removing: ");
		Serial.println(monsterSerial);

		zone.UpdateCurrentMonster(monsterSerial, Enums::NoCard);
		return;
	}

	position = zone.ReadCurrentMonsterPosition();
	zone.UpdateCurrentMonster(cardSerialNumber, position);
}