#include "ZoneHandler.h"
#include "Wire.h"

//#define DEBUG_ZH

ZoneHandler::ZoneHandler(SmartDuelEventHandler& eventHandler, PeripheralsHandler& peripheralsHandler) {
	_eventHandler = &eventHandler;
	_peripheralsHandler = &peripheralsHandler;
}

// TODO: Check which pin can be used as analog
void ZoneHandler::Initialize(byte numZones, PN532& reader) {
	pinMode(A0, INPUT);

	for (byte i = 0; i < numZones; i++) {
		Zones[i].Initialize(reader);
	}
}

void ZoneHandler::CheckForNewEvents(int zoneNumber) {

	Zones[zoneNumber].CheckForCardPresenceChanges();

	if (Zones[zoneNumber].monsterZone.Card.hasCardPresenceChanged) {
		delay(250); // delay to ensure card is fully placed
		HandleCardPresenceChange(Zones[zoneNumber], Enums::Monster);
		_eventHandler->HandleOutgoingEvent(
			Zones[zoneNumber].monsterZone.Card.serialNum,
			Zones[zoneNumber].monsterZone.Card.copyNum,
			Zones[zoneNumber].monsterZone.ZoneName,
			Zones[zoneNumber].monsterZone.Card.cardPosition);
	}

	if (Zones[zoneNumber].spellZone.Card.hasCardPresenceChanged) {
		delay(250); // delay to ensure card is fully placed
		HandleCardPresenceChange(Zones[zoneNumber], Enums::SpellTrap);
		_eventHandler->HandleOutgoingEvent(
			Zones[zoneNumber].spellZone.Card.serialNum,
			Zones[zoneNumber].spellZone.Card.copyNum,
			Zones[zoneNumber].spellZone.ZoneName,
			Zones[zoneNumber].spellZone.Card.cardPosition);
	}
}

void ZoneHandler::HandleCardPresenceChange(DualCardZone& zone, Enums::SingleCardZoneType zoneType) {
	switch (zoneType) {
	case Enums::Monster:
		zone.UpdateCurrentMonster();
		break;
	case Enums::SpellTrap:
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