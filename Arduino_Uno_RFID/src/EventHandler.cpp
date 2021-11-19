#include "Arduino.h"
#include "EventHandler.h"
#include "Core\Zone.h"
#include "Core\Entities\Enums.h"

EventHandler::EventHandler() 
{
}

String EventHandler::FormatEventInfo(DualCardZone zone, Enums::Events eventType, Enums::SensorType sensor)
{
	String comma = ",";
	String eventInfo;
	Monster monster = zone.GetCurrentMonster();
	Spell spell = zone.GetCurrentSpell();

	eventInfo.concat(eventType);
	eventInfo.concat(comma);

	// Card Zone
	if (sensor != Enums::SpellTrap) {
		eventInfo.concat(zone.ZoneNumber);
		eventInfo.concat(comma);
	}
	else {
		eventInfo.concat(zone.ZoneNumber + 3);
		eventInfo.concat(comma);
	}

	// Card ID and Position
	if (sensor != Enums::SpellTrap) {
		eventInfo.concat(monster.GetSerialNumber());
		eventInfo.concat(comma);
		eventInfo.concat(monster.GetPosition());
	}
	else {
		eventInfo.concat(spell.GetSerialNumber() + comma);
		eventInfo.concat(spell.GetPosition());
	}

	return eventInfo;
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