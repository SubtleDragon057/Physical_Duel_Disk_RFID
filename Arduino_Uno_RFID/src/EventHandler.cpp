#include "Arduino.h"
#include "EventHandler.h"
#include "Core\Zone.h"
#include "Core\Entities\Enums.h"

EventHandler::EventHandler(bool debug) 
{
	_debug = debug;
}

String EventHandler::GetFormattedEventData(DualCardZone zone, Enums::SensorType sensor)
{
	String eventData = "";
	Monster monster = zone.GetCurrentMonster();
	Spell spell = zone.GetCurrentSpell();

	eventData.concat(SetEventType(zone, sensor));

	// Card Zone
	if (sensor != Enums::SpellTrap) {
		eventData.concat(zone.ZoneNumber);
	}
	else {
		eventData.concat(zone.ZoneNumber + 3);
	}

	// Card ID and Position
	if (sensor != Enums::SpellTrap) {
		eventData.concat(monster.GetSerialNumber());
		eventData.concat(monster.GetPosition());
	}
	else {
		eventData.concat(spell.GetSerialNumber());
		eventData.concat(spell.GetPosition());
	}

	// Handle Error
	if (eventData == "205" || eventData == "215" || eventData == "225") {
		Serial.print("[ERROR] Zone: ");
		Serial.print(eventData[1]);
		Serial.println(" had a malfunction");

		eventData = "";
	}

	return eventData;
}

Enums::Events EventHandler::SetEventType(DualCardZone zone, Enums::SensorType sensor) {
	if (sensor == Enums::SpellTrap) {
		Spell spell = zone.GetCurrentSpell();

		if (spell.GetPosition() == Enums::NoCard) {
			return Enums::Events::Remove;
		}

		return Enums::Events::SetSpellTrap;
	}

	Monster monster = zone.GetCurrentMonster();
	if (monster.GetPosition() == Enums::NoCard) {
		return Enums::Events::Remove;
	}

	return Enums::Events::Summon;
}