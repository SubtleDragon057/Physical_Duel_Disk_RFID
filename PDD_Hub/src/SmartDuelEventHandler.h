#ifndef SmartDuelEventHandler_h
#define SmartDuelEventHandler_h
#include "Arduino.h"
#include "PeripheralsHandler.h"
#include "StorageHandler.h"
#include "Core\SmartDuelServer.h"
#include "Secrets.h"
#include "Features\SpeedDuel.h"
#include "Core\Utils\JSONUtility.h"

class SmartDuelEventHandler {
private:
	SmartDuelServer _server;
	PeripheralsHandler* _peripheralsHandler;
	StorageHandler* _storageHandler;
	SECRETS _secrets;
	SpeedDuel _speedDuel;
	JSONUtility _jsonUtility;

	int _eventStartTime;
	bool _uiEventActive = false;
	const byte _numButtonEvents = 6;

	// Duel Room Functions
	void HandleLobby(int buttonEvents[]);
	void EnterWriteMode(int deckList[]);
	void HandleDuelRoom(int buttonEvents[]);
	void HandleButtonInteraction(int buttonEvents[], bool isInBattle = false);

	// Server Functions
	void HandleIncomingRoomEvents();
	void HandleIncomingCardEvents();
	void HandleIncomingDuelistEvents();

	void DisplayMessageOnScreen(PeripheralsHandler::UI_Type uiType, String message);
	String GetTargetZoneName(int multiButtonEvent, int zoneNumber);

public:
	String SocketID;
	Enums::DuelRoomState DuelRoomState = Enums::Lobby;
	
	SmartDuelEventHandler(PeripheralsHandler &peripheralsHandler, StorageHandler &storageHandler);

	// Duel Room Functions
	void HandleDuelRoomState(int buttonEvents[]);
	void HandleMultiButtonEvent(int buttonEventType);
	void HandlePhaseChange();

	// Server Functions
	bool IsConnected() { return _server.isConnected; }
	void Connect(bool useEncryption = false);
	void ListenToServer();
	void HandleOutgoingEvent(int cardID, int copyNum, String zoneName, String cardPosition);
};

#endif