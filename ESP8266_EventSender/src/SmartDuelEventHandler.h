#ifndef SmartDuelEventHandler_h
#define SmartDuelEventHandler_h
#include "Arduino.h"
#include "Core\SmartDuelServer.h"
#include "Features\Lobby.h"
#include "Features\DuelRoom.h"

class SmartDuelEventHandler {
private:
	SmartDuelServer _server;
	Lobby _lobby;
	DuelRoom _duelRoom;
	bool _debug;

public:
	bool IsInDuelRoom = false;
	bool IsDueling = false;
	String SocketID;
	
	SmartDuelEventHandler(bool debug);

	// Lobby Functions
	void InitializeLobby(int deckList[]);
	void HandleLobby(int buttonEvents[]);

	// Duel Room Functions
	void HandleDuelRoom(int buttonEvents[]);

	// Server Functions
	void Connect(String socketIP, int socketPort);
	String GetSocketId();
	void SendEvent(String eventData);
	void ListenToServer();
	void HandleIncomingEvents();
};

#endif