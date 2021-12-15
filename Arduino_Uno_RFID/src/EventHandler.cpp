#include "Arduino.h"
#include "EventHandler.h"
#include "Core\Zone.h"
#include "Core\Entities\Enums.h"

EventHandler::EventHandler(bool debug) 
{
	_debug = debug;
}

String EventHandler::GetFormattedEventData(DualCardZone &zone, int sensor)
{
	String eventData = "";
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
		eventData.concat(zone.MonsterSerial);
		eventData.concat(zone.MonsterPosition);
	}
	else {
		eventData.concat(zone.SpellSerial);
		eventData.concat(zone.SpellPosition);
	}

	// Handle Error
	if (eventData.length() < 11) {
		Serial.print(F("[ERROR] Zone: "));
		Serial.print(eventData[1]);
		Serial.println(F(" had a malfunction!"));
		eventData = "";
	}

	return eventData;
}

Enums::Events EventHandler::SetEventType(DualCardZone zone, int sensor) {
	if (sensor == Enums::SpellTrap) {
		return zone.SpellPosition == Enums::NoCard 
			? Enums::Events::Remove 
			: Enums::Events::SetSpellTrap;
	}

	return zone.MonsterPosition == Enums::NoCard
		? Enums::Events::Remove
		: Enums::Events::Summon;
}