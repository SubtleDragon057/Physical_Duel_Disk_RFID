#include "Arduino.h"
#include "ZoneHandler.h"
#include "Core\Entities\Enums.h"
#include "Core\Entities\YgoCard.h"
#include "Core\Entities\Components.h"
#include "Core\Zone.h"

ZoneHandler::ZoneHandler(bool debug) 
{
	_debug = debug;
}

void ZoneHandler::Initialize(byte numZones, byte readerPins[], ProximitySensor attackSensors[], 
	AnalogIR defenceSensors[], AnalogIR spellSensors[]) {
	
	for (int i = 0; i < numZones; i++) {
		Zones[i].Initialize(i,
			readerPins[i], 
			readerPins[4],
			attackSensors[i],
			defenceSensors[i],
			spellSensors[i]);

		delay(10);
	}
}

Enums::SensorType ZoneHandler::CheckForTrippedSensor(int zoneNumber) {
	
	Enums::SensorType isNewCardPresent = Zones[zoneNumber].isNewCardPresent();
	if (isNewCardPresent != Enums::None) {
		if (_debug) {
			Serial.print("[DEBUG] ");
			Serial.print(zoneNumber);
			Serial.print(": Tripped Sensor: ");
			Serial.println(isNewCardPresent);
		}
		
		delay(100); // delay to ensure card is fully placed
		CheckRFIDReader(Zones[zoneNumber], isNewCardPresent);
	}

	return isNewCardPresent;
}

void ZoneHandler::CheckRFIDReader(DualCardZone &zone, Enums::SensorType sensor) {
	bool isMonster = false;
	Enums::Events eventType = Enums::NoEvent;

	Serial.println("[DEBUG] ZoneHandler CheckRFIDReader Entered");
	
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
		position = GetSpellPosition(zone.SpellSensor);
		zone.UpdateCurrentSpell(cardSerialNumber, position);
		return; // TODO: Handle Spell Removal
	}

	if (isRemoval) {
		zone.UpdateCurrentMonster(zone.GetCurrentMonster().GetSerialNumber(), Enums::NoCard);
		return;
	}

	position = GetMonsterPosition(zone.AttackSensor, zone.DefenceSensor);
	zone.UpdateCurrentMonster(cardSerialNumber, position);
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