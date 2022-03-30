#include "ZoneHandler.h"
#include "Wire.h"

//#define DEBUG_ZH

ZoneHandler::ZoneHandler(EventHandler& eventHandler) {	
	_eventHandler = &eventHandler;
}

void ZoneHandler::Initialize(byte numZones, PN532& reader) {

	byte error;
	do {
		Wire.beginTransmission(_multiplexerAddress);
		error = Wire.endTransmission();
		delay(50);
	} while (error != 0);

	pinMode(2, OUTPUT);
	pinMode(3, OUTPUT);
	pinMode(4, OUTPUT);
	pinMode(5, OUTPUT);
	pinMode(A0, INPUT);

	for (byte i = 0; i < numZones; i++) {
		Zones[i].Initialize(reader);
	}
}

uint8_t ZoneHandler::CheckForNewEvents(int zoneNumber) {
	
	uint8_t eventType = 0;
	Zones[zoneNumber].CheckForCardPresenceChanges();

	if (Zones[zoneNumber].monsterZoneCard.hasCardPresenceChanged) {
		delay(250); // delay to ensure card is fully placed
		HandleCardPresenceChange(Zones[zoneNumber], Enums::MonsterZone);
		Events[zoneNumber].monsterZoneEvent =
			_eventHandler->GetFormattedEventData(Zones[zoneNumber], Enums::MonsterZone);
		eventType = 1;
	}

	if (Zones[zoneNumber].spellZoneCard.hasCardPresenceChanged) {
		delay(250); // delay to ensure card is fully placed
		HandleCardPresenceChange(Zones[zoneNumber], Enums::SpellTrapZone);
		Events[zoneNumber].spellZoneEvent =
			_eventHandler->GetFormattedEventData(Zones[zoneNumber], Enums::SpellTrapZone);
		eventType = 2;
	}

	if (Zones[zoneNumber].monsterZoneCard.hasCardPresenceChanged &&
		Zones[zoneNumber].spellZoneCard.hasCardPresenceChanged) {
		eventType = 3;
	}

	return eventType;
}

void ZoneHandler::HandleCardPresenceChange(DualCardZone& zone, Enums::SingleCardZoneType zoneType) {
	switch (zoneType) {
		case Enums::MonsterZone:
			zone.UpdateCurrentMonster();
			break;
		case Enums::SpellTrapZone:
			zone.UpdateCurrentSpell();
			break;
		default:
			Serial.println(F("[ERROR] zoneType Not Defined!"));
	}
}

bool ZoneHandler::EnableWriteMode(String cardID) {
//	if (cardID == "") return false;
//	byte useZone = 0;
//	
//	SelectMultiplexerAddress(useZone);
//	bool cardAvailable = Zones[useZone].ScanForNewCard();
//	if (!cardAvailable) {
//		Serial.println("Waiting For Card");
//		delay(2000);
//		return false;
//	}
//
//	Serial.println("Card Found!");
//	delay(2000); // Ensure Card is fully placed
//
//	bool success = Zones[useZone].WriteRFIDTag(cardID);
//	
//	Serial.print("Written ID: ");
//	if (success) Serial.println(Zones[useZone].GetCardSerialNumber());
//	else {
//		Serial.println("Write Failed");
//		return false;
//	}
//
//	while(Zones[useZone].ScanForNewCard()) {
//		Serial.println("Please Remove Card");
//		delay(2000);
//	}
//
	return false;
}