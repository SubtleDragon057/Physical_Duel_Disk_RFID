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

	DualCardZone _zone1(0);

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

//byte _muxChannels[16][4] = {
	//{0,0,0,0}, //channel 0
	//{1,0,0,0}, //channel 1
	//{0,1,0,0}, //channel 2
	//{1,1,0,0}, //channel 3
	//{0,0,1,0}, //channel 4
	//{1,0,1,0}, //channel 5
	//{0,1,1,0}, //channel 6
	//{1,1,1,0}, //channel 7
	//{0,0,0,1}, //channel 8
	//{1,0,0,1}, //channel 9
	//{0,1,0,1}, //channel 10
	//{1,1,0,1}, //channel 11
	//{0,0,1,1}, //channel 12
	//{1,0,1,1}, //channel 13
	//{0,1,1,1}, //channel 14
	//{1,1,1,1}  //channel 15
	//};