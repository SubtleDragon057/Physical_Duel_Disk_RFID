#ifndef Lobby_h
#define Lobby_h
#include "Arduino.h"
#include "ArduinoJson.h"

class Lobby {
private:
	String HandleCreateRoomEvent(int deckList[]);
	String HandleJoinRoomEvent(int deckList[]);

public:
	static String CurrentRoom;

	Lobby();

	String CheckLobbyForAction(int buttonEvents[], int deckList[]);
	void UpdateCurrentRoom(String roomName);
};

#endif