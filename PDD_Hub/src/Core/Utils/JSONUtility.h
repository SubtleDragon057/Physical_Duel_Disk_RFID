#ifndef JSONUtility_h
#define JSONUtility_h
#include "Arduino.h"

class JSONUtility {
public:
	JSONUtility();

	String HandleCreateRoomEvent(int deckList[]);
	String HandleJoinRoomEvent(int deckList[]);
	String HandleCloseRoomEvent(String roomName);

	String GetCardEventAsJSON(String socketID, String eventName, int cardID, int copyNumber, 
		String zoneName = "", String cardPosition = "");
	String GetDuelistEventAsJSON(String socketID, String eventName);
	String GetPhaseEventAsJSON(String socketId, String eventName, String newPhase = String());
};

#endif