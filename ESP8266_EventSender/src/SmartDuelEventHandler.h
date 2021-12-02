#ifndef SmartDuelEventHandler_h
#define SmartDuelEventHandler_h
#include "Arduino.h"
#include "Core\SmartDuelServer.h"
#include "Features\Lobby.h"
#include "Features\DuelRoom.h"
#include "Features\SpeedDuel.h"
#include "Core\Utils\JSONUtility.h"
#include "Core\Entities\Enums.h"

class SmartDuelEventHandler {
private:
	SmartDuelServer _server;
	Lobby _lobby;
	DuelRoom _duelRoom;
	SpeedDuel _speedDuel;
	JSONUtility _jsonUtility;
	bool _debug;

	String GetTargetZoneName(int multiButtonEvent, int zoneNumber);

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

	// Speed Duel Functions
	bool HasAttackTarget() { return _speedDuel.HasAttackTarget; }
	void HandleButtonInteraction(int buttonEvents[], bool isInBattle = false);
	void HandleMultiButtonEvent(int buttonEventType);

	// Server Functions
	void Connect(String socketIP, int socketPort);
	void ListenToServer();
	void HandleIncomingRoomEvents();
	void HandleIncomingCardEvents();
	void HandleOutgoingEvent(String eventData);
};

#endif