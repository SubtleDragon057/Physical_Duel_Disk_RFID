#ifndef SmartDuelEventWrapper_h
#define SmartDuelEventWrapper_h
#include "Arduino.h"
#include "ArduinoJson.h"

class SmartDuelEventWrapper {
private:
	String PlayCardEvent(String socketID, String eventInfo[]);
	String GetZoneName(String zone, String eventType);
	String GetCardPosition(String positionNumber);
	int GetCardID(String cardID);

public:
	SmartDuelEventWrapper();

	String GetCardEventAsJSON(String socketID, String data);
	String GetAttackEventAsJSON(String socketID, int monsterID, int copyNumber, String targetZone);
	String GetDeclareEventAsJSON(String socketID, int cardID, int copyNumber);
	String GetTargetZoneName(int zoneNumber);
};

#endif