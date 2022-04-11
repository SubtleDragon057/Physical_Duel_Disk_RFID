#ifndef ZoneHandler_h
#define ZoneHandler_h
#include "Arduino.h"
#include "Core\Zone.h"
#include "Core\Entities\Enums.h"
#include "Core\Entities\EventData.h"
#include "PN532.h"
#include "EventHandler.h"

class ZoneHandler {
private:	
	const uint8_t _multiplexerAddress = 0x70;
	EventHandler* _eventHandler;

	void HandleCardPresenceChange(DualCardZone& zone, Enums::SingleCardZoneType zoneType);

public:

	DualCardZone Zones[3] = {
		DualCardZone(0),
		DualCardZone(1),
		DualCardZone(2)
	};

	EventData Events[3] = {
		EventData(),
		EventData(),
		EventData()
	};
	
	ZoneHandler(EventHandler& eventHandler);

	void Initialize(byte numZones, PN532& reader);
	uint8_t CheckForNewEvents(int zoneNumber);
	bool EnableWriteMode(String cardID);
};

#endif