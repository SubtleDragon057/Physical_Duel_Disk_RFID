#include "EventHandler.h"

#define DEBUG_EH

EventHandler::EventHandler() 
{
}

String EventHandler::GetFormattedEventData(DualCardZone &zone, Enums::SingleCardZoneType zoneType) {
	String eventData = "";
	eventData.concat(SetEventType(zone, zoneType));

	// Card Zone
	if (zoneType != Enums::SpellTrapZone) {
		eventData.concat(zone.ZoneNumber);
	}
	else {
		eventData.concat(zone.ZoneNumber + 3);
	}

	// Card ID and Position
	if (zoneType != Enums::SpellTrapZone) {
		eventData.concat(zone.monsterZoneCard.serialNum);
		eventData.concat(zone.monsterZoneCard.cardPosition);
	}
	else {
		eventData.concat(zone.spellZoneCard.serialNum);
		eventData.concat(zone.spellZoneCard.cardPosition);
	}

	// Handle Error
	if (eventData.length() < 11) {
#ifdef DEBUG_EH
		Serial.print(F("[ERROR] Zone: "));
		Serial.print(eventData[1]);
		Serial.println(F(" had a malfunction!"));
#endif // DEBUG_EH

		eventData = "";
	}

	return eventData;
}

Enums::Events EventHandler::SetEventType(DualCardZone zone, int sensor) {
	if (sensor == Enums::SpellTrapZone) {
		return zone.spellZoneCard.cardPosition == Enums::NoCard 
			? Enums::Events::Remove 
			: Enums::Events::SetSpellTrap;
	}

	return zone.monsterZoneCard.cardPosition == Enums::NoCard
		? Enums::Events::Remove
		: Enums::Events::Summon;
}