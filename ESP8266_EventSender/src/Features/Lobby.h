#ifndef Lobby_h
#define Lobby_h
#include "Arduino.h"
#include "ArduinoJson.h"

class Lobby {
private:
	int* _decklist;

	String HandleCreateRoomEvent();
	String HandleJoinRoomEvent();

public:
	static String CurrentRoom;

	Lobby();
	void Initialize(int decklist[]);

	String CheckLobbyForAction(int buttonEvents[]);
	void UpdateCurrentRoom(String roomName);
};

#endif