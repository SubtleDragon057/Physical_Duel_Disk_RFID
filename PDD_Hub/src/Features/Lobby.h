#ifndef Lobby_h
#define Lobby_h
#include "Arduino.h"

class Lobby {
private:	
	String HandleCreateRoomEvent(int deckList[]);
	String HandleJoinRoomEvent(int deckList[]);

public:

	Lobby();

	String CheckLobbyForAction(int buttonEvents[], int deckList[]);
};

#endif