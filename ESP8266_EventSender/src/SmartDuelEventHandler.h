#ifndef SmartDuelEventHandler_h
#define SmartDuelEventHandler_h
#include "Arduino.h"
#include "CommunicationsHandler.h"
#include "Core\SmartDuelServer.h"
#include "Features\Lobby.h"
#include "Features\DuelRoom.h"
#include "Features\SpeedDuel.h"
#include "Core\Utils\JSONUtility.h"

class SmartDuelEventHandler {
private:
	SmartDuelServer _server;
	CommunicationsHandler * _communicationsHandler;
	Lobby _lobby;
	DuelRoom _duelRoom;
	SpeedDuel _speedDuel;
	JSONUtility _jsonUtility;

	int _eventStartTime;
	bool _uiEventActive = false;
	const byte _numButtonEvents = 6;

	String GetTargetZoneName(int multiButtonEvent, int zoneNumber);

public:
	bool IsInDuelRoom = false;
	bool IsDueling = false;
	String SocketID;
	
	SmartDuelEventHandler(CommunicationsHandler &communicationsHandler);

	// Lobby Functions
	void HandleLobby(int buttonEvents[], int deckList[]);

	// Duel Room Functions
	void HandleDuelRoom(int buttonEvents[]);

	// Speed Duel Functions
	bool HasAttackTarget() { return _speedDuel.HasAttackTarget; }
	void HandleButtonInteraction(int buttonEvents[], bool isInBattle = false);
	void HandleMultiButtonEvent(int buttonEventType);

	void HandlePhaseChange();

	// Server Functions
	bool IsConnected() { return _server.isConnected; }
	void Connect(String socketIP, int socketPort);
	void ListenToServer();
	void HandleIncomingRoomEvents();
	void HandleIncomingCardEvents();
	void HandleOutgoingEvent(String eventData);
};

#endif