#ifndef _ZoneHandler_h
#define _ZoneHandler_h
#include "arduino.h"
#include "SmartDuelEventHandler.h"
#include "PeripheralsHandler.h"
#include "Core\Entities\Enums.h"
#include "Features\Entities\Zones.h"
#include "PN532.h"

class ZoneHandler {
private:
	SmartDuelEventHandler* _eventHandler;
	PeripheralsHandler* _peripheralsHandler;

	void HandleCardPresenceChange(DualCardZone& zone, Enums::SingleCardZoneType zoneType);

public:

	DualCardZone Zones[3] = {
		DualCardZone(0, "mainMonster1", "spellTrap1"),
		DualCardZone(1, "mainMonster2", "spellTrap2"),
		DualCardZone(2, "mainMonster3", "spellTrap3")
	};

	ZoneHandler(SmartDuelEventHandler& eventHandler, PeripheralsHandler& peripheralsHandler);

	void Initialize(byte numZones, PN532& reader);
	void CheckForNewEvents(int zoneNumber);
	bool EnableWriteMode(String cardID);
};

#endif