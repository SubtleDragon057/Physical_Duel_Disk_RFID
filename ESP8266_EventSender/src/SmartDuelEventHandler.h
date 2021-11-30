#ifndef SmartDuelEventHandler_h
#define SmartDuelEventHandler_h
#include "Arduino.h"
#include "Core\SmartDuelServer.h"
#include "Features\Lobby.h"
#include "Features\DuelRoom.h"
#include "Features\DuelState.h"
#include "Wrappers\SmartDuelEventWrapper.h"
#include "Core\Entities\Enums.h"

class SmartDuelEventHandler {
private:
	SmartDuelServer _server;
	DuelState _duelState;
	Lobby _lobby;
	DuelRoom _duelRoom;
	SmartDuelEventWrapper _eventWrapper;
	int _attackingMonster;
	bool _debug;

public:
	bool IsInDuelRoom = false;
	bool IsDueling = false;
	bool HasAttackTarget = true;
	String SocketID;
	
	SmartDuelEventHandler(bool debug);

	// Lobby Functions
	void InitializeLobby(int deckList[]);
	void HandleLobby(int buttonEvents[]);

	// Duel Room Functions
	void HandleDuelRoom(int buttonEvents[]);

	// Speed Duel Functions
	void HandleButtonInteraction(int buttonEvents[]);
	void HandleActivateSpell(int zoneNumber);
	void HandleActivateMonsterEffect(int zoneNumber);
	void HandleMonsterAttack(int zoneNumber);
	void HandleAttackEvent(int buttonEvents[]);
	bool CheckForValidTarget(int zoneNumber);

	// Server Functions
	void Connect(String socketIP, int socketPort);
	void ListenToServer();
	void HandleIncomingRoomEvents();
	void HandleIncomingCardEvents();
	void HandleOutgoingEvent(String eventData);
};

#endif