#include "Arduino.h"
#include "EventHandler.h"
#include "Core\Zone.h"
#include "Core\Entities\Enums.h"
#include "Core\Entities\YgoCard.h"

EventHandler::EventHandler(bool debug) 
{
	_debug = debug;
}

String EventHandler::GetFormattedEventData(DualCardZone &zone, Enums::SensorType sensor)
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
	if (eventData.length() == 3) {
		Serial.print(F("[ERROR] Zone: "));
		Serial.print(eventData[1]);
		Serial.println(F(" had a malfunction!"));
		eventData = "";
	}

	return eventData;
}

Enums::Events EventHandler::SetEventType(DualCardZone zone, Enums::SensorType sensor) {
	if (sensor == Enums::SpellTrap) {
		Spell spell = zone.GetCurrentSpell();

		return spell.GetPosition() == Enums::NoCard 
			? Enums::Events::Remove 
			: Enums::Events::SetSpellTrap;
	}

	Monster monster = zone.GetCurrentMonster();
	return monster.GetPosition() == Enums::NoCard
		? Enums::Events::Remove
		: Enums::Events::Summon;
}