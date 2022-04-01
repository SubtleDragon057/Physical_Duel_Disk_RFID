#ifndef JSONUtility_h
#define JSONUtility_h
#include "Arduino.h"

class JSONUtility {
private:	
	String GetZoneName(String zone, String eventType);
	String GetCardPosition(String positionNumber);
	int GetCardID(String cardID);

public:
	JSONUtility();

	String HandleCreateRoomEvent(int deckList[]);
	String HandleJoinRoomEvent(int deckList[]);
	String HandleCloseRoomEvent(String roomName);

	String GetCardEventFromArduino(String socketID, String data);
	String GetCardEventAsJSON(String socketID, String eventName, int cardID, int copyNumber, 
		String zoneName = "", String cardPosition = "");
	String GetDuelistEventAsJSON(String socketID, String eventName);
	String GetPhaseEventAsJSON(String socketId, String eventName, String newPhase = String());
};

#endif