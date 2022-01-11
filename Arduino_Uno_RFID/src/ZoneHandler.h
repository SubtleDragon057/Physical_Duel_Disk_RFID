#ifndef ZoneHandler_h
#define ZoneHandler_h
#include "Arduino.h"
#include "Core\Zone.h"
#include "Core\Entities\Enums.h"
#include "PN532.h"

//#define DEBUG_ZH

class ZoneHandler {
private:
	const uint8_t _multiplexerAddress = 0x70;
	byte _muxChannels[16][4] = {
	{0,0,0,0}, //channel 0
	{1,0,0,0}, //channel 1
	{0,1,0,0}, //channel 2
	{1,1,0,0}, //channel 3
	{0,0,1,0}, //channel 4
	{1,0,1,0}, //channel 5
	{0,1,1,0}, //channel 6
	{1,1,1,0}, //channel 7
	{0,0,0,1}, //channel 8
	{1,0,0,1}, //channel 9
	{0,1,0,1}, //channel 10
	{1,1,0,1}, //channel 11
	{0,0,1,1}, //channel 12
	{1,0,1,1}, //channel 13
	{0,1,1,1}, //channel 14
	{1,1,1,1}  //channel 15
	};

	void SelectMultiplexerAddress(uint8_t address);
	void CheckRFIDReader(DualCardZone &zone, int sensor);

public:
		
	DualCardZone Zones[3] = {
		DualCardZone(),
		DualCardZone(),
		DualCardZone()
	};
	
	ZoneHandler();

	void Initialize(byte numZones, PN532& reader, const byte attackSensorAddresses[],
		const byte defenceSnesorPins[], const byte spellSensorAddresses[]);
	void CheckForTrippedSensor(int zoneNumber);
	void HandleUpdateCard(DualCardZone& zone, int sensorType, Enums::CardPosition position);
	void HandleRemoveCard(DualCardZone& zone, int sensorType);
	bool EnableWriteMode(String cardID);
};

#endif