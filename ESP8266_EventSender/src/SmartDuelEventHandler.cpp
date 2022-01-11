#include "SmartDuelEventHandler.h"
#include "Core\Entities\Enums.h"
#include "Features\Entities\EventData.h"

SmartDuelEventHandler::SmartDuelEventHandler(CommunicationsHandler& communicationsHandler)
{
	_communicationsHandler = &communicationsHandler;
}

void SmartDuelEventHandler::HandleLobby(int buttonEvents[], int deckList[])
{
	String text[] = { "" };
	_communicationsHandler->Display(CommunicationsHandler::UI_Lobby, text);
	
	String data = _lobby.CheckLobbyForAction(buttonEvents, deckList);
	if (data == "NoAction") return;

	if (data == "writeMode") {
		EnterWriteMode(deckList);
		return;
	}

	_server.SendEvent(data);
}

// TODO: This function will lock up the system for its duration
void SmartDuelEventHandler::EnterWriteMode(int deckList[]) {
	String text[] = { "Write Mode" };
	_communicationsHandler->Display(CommunicationsHandler::UI_WriteMode, text);

	Serial.println("Decklist With Copy Num:");
	String deckListWithCopyNum[35];
	for (byte i = 0; i < 35; i++) {
		if (deckList[i] == 0) continue;
		int copyNum = 0;

		for (byte j = 0; j < 35; j++) {
			if (deckList[i] != deckList[j]) continue;
			copyNum++;
		}

		deckListWithCopyNum[i] = String(copyNum) + String(deckList[i]);
		deckList[i] = 0;
		Serial.println(deckListWithCopyNum[i]);
	}

	_communicationsHandler->EnableWriteMode();
	Serial.printf("Sending Deck\n");

	for (byte i = 0; i < 35; i++) {
		if (deckListWithCopyNum[i] == 0) continue;
		
		String cardNumber[] = { deckListWithCopyNum[i].substring(1) };
		_communicationsHandler->Display(CommunicationsHandler::UI_WriteMode, cardNumber);
		_communicationsHandler->TransmitCard(deckListWithCopyNum[i]);
	}

	text[1] = "Done! Returning to Lobby";
	_communicationsHandler->Display(CommunicationsHandler::UI_WriteMode, text);
	delay(1000);
}

void SmartDuelEventHandler::HandleDuelRoom(int buttonEvents[]) {
	String text[] = { _duelRoom.RoomName };
	_communicationsHandler->Display(CommunicationsHandler::UI_Lobby, text);
	
	String data = _duelRoom.CheckDuelRoomForAction(buttonEvents);
	if (data == "NoAction") return;

	_server.SendEvent(data);
}

void SmartDuelEventHandler::HandleButtonInteraction(int buttonEvents[], bool isInBattle) {
	EventData newData;
	
	for (byte i = 0; i < _numButtonEvents; i++) {
		if (buttonEvents[i] == Enums::ButtonClicks::NoChange) continue;

		if (isInBattle) {
			String zoneName = GetTargetZoneName(buttonEvents[5], i);
			newData = _speedDuel.HandleMonsterAttack(SocketID, zoneName);
			break;
		}

		switch (buttonEvents[i]) {
			case Enums::ButtonClicks::Single:
				newData = _speedDuel.HandleActivateSpell(SocketID, i);
				continue;
			case Enums::ButtonClicks::Double:
				newData = _speedDuel.HandleActivateMonsterEffect(SocketID, i);
				continue;
			case Enums::ButtonClicks::Hold:
				_speedDuel.HandleAttackEvent(SocketID, i);
				continue;
			case Enums::ButtonClicks::Multi01:
			case Enums::ButtonClicks::Multi34:
			case Enums::ButtonClicks::Multi04:
				HandleMultiButtonEvent(buttonEvents[i]);
				return;
			case Enums::ButtonClicks::Multi13:
				HandlePhaseChange();
				return;
		}
	}

	String eventData = _jsonUtility.GetCardEventAsJSON(SocketID, newData.EventName, 
		newData.CardID, newData.CopyNumber, newData.ZoneName);
	_server.SendEvent(eventData);
}

void SmartDuelEventHandler::HandleMultiButtonEvent(int buttonEventType) {
	String eventName;

	switch (buttonEventType) {
		case Enums::ButtonClicks::Multi01:
			eventName = "duelist:flip-coin";
			break;
		case Enums::ButtonClicks::Multi34:
			eventName = "duelist:roll-dice";
			break;
		default:
			eventName = "duelist:flip-coin";
			break;
	}
	
	String eventData = _jsonUtility.GetDuelistEventAsJSON(SocketID, eventName);
	_server.SendEvent(eventData);
}

void SmartDuelEventHandler::HandlePhaseChange() {
	String newData = _speedDuel.HandleChangePhase();
	if (newData == String()) return;

	if (newData == "passTurn") {
		String eventData = _jsonUtility.GetPhaseEventAsJSON(SocketID, "duelist:end-turn");
		_server.SendEvent(eventData);
		return;
	}

	String eventData = _jsonUtility.GetPhaseEventAsJSON(SocketID, "duelist:declare-phase", newData);
	_server.SendEvent(eventData);
}

void SmartDuelEventHandler::Connect(String socketIP, int socketPort) {
	String text[] = { "Locating Local Server" };
	_communicationsHandler->Display(CommunicationsHandler::UI_Init, text);
	
	_server.Initialize(socketIP, socketPort);
}

// TODO: Gives 404 code and disconnects
void SmartDuelEventHandler::ConnectSecure(String socketIP, int socketPort) {
	String text[] = { "Locating Duel Servers" };
	_communicationsHandler->Display(CommunicationsHandler::UI_Init, text);

	_server.InitializeSSL(socketIP, socketPort);
}

void SmartDuelEventHandler::ListenToServer() {
	String socketID = _server.GetSocketId();
	if (_server.isConnected && (SocketID == NULL || SocketID != socketID)) {
		SocketID = socketID;
	}
	
	_server.ListenToServer();
	HandleIncomingRoomEvents();
	HandleIncomingCardEvents();
}

void SmartDuelEventHandler::HandleIncomingRoomEvents() {		
	
	if (SmartDuelServer::EventName == "room:create" ||
		SmartDuelServer::EventName == "room:join") {
		_duelRoom.UpdateCurrentRoom(SmartDuelServer::RoomName);
		IsInDuelRoom = true;
		SmartDuelServer::EventName = "Waiting";
	}	
	else if (SmartDuelServer::EventName == "room:close") {
		IsInDuelRoom = false;
		IsDueling = false;
		_speedDuel.ClearDuelStates();
		SmartDuelServer::EventName = "Waiting";
	}
	else if (SmartDuelServer::EventName == "room:start") {
		_duelRoom.UpdateCurrentRoom(SmartDuelServer::RoomName);
		IsInDuelRoom = true;
		_speedDuel.UpdateDuelistIDs(
			SocketID,
			SmartDuelServer::DuelistID,
			SmartDuelServer::EventData);
		IsDueling = true;

		bool isOpponentsTurn = SmartDuelServer::EventData != SocketID;
		_speedDuel.UpdatePhase("drawPhase", isOpponentsTurn);
		_communicationsHandler->StartDuelDisk(_speedDuel.GetPhase());
		SmartDuelServer::EventName = "Waiting";
	}
}

void SmartDuelEventHandler::HandleIncomingCardEvents() {
	if (_uiEventActive && (millis() - _eventStartTime) > 4000) {
		_uiEventActive = false;
		String currentPhase[] = { _speedDuel.GetPhase() };
		_communicationsHandler->Display(CommunicationsHandler::UI_SpeedDuel, currentPhase);
	}
	
	if (SmartDuelServer::EventName == "card:play") {
		_speedDuel.UpdateDuelState(
			SmartDuelServer::DuelistID,
			SmartDuelServer::CardID,
			SmartDuelServer::CopyNumber,
			SmartDuelServer::EventData);
		SmartDuelServer::EventName = "Waiting";
	}
	else if (SmartDuelServer::EventName == "duelist:declare-phase") {
		bool isOpponentsTurn = SmartDuelServer::DuelistID != SocketID;

		_speedDuel.UpdatePhase(SmartDuelServer::EventData, isOpponentsTurn);
		String currentPhase[] = { _speedDuel.GetPhase() };
		_communicationsHandler->Display(CommunicationsHandler::UI_SpeedDuel, currentPhase);
		SmartDuelServer::EventName = "Waiting";
	}
	else if (SmartDuelServer::EventName == "duelist:end-turn") {
		bool isOpponentsTurn = SmartDuelServer::DuelistID == SocketID;

		_speedDuel.UpdatePhase("drawPhase", isOpponentsTurn);
		String currentPhase[] = { _speedDuel.GetPhase() };
		_communicationsHandler->Display(CommunicationsHandler::UI_SpeedDuel, currentPhase);
		SmartDuelServer::EventName = "Waiting";
	}
	else if (SmartDuelServer::EventName == "duelist:flip-coin") {
		String text[] = { "Flip: " + SmartDuelServer::EventData };
		_communicationsHandler->Display(CommunicationsHandler::UI_SpeedDuel, text);
		_uiEventActive = true;
		_eventStartTime = millis();
		SmartDuelServer::EventName = "Waiting";
	}
	else if (SmartDuelServer::EventName == "duelist:roll-dice") {
		String text[] = { "Roll: " + SmartDuelServer::EventData };
		_communicationsHandler->Display(CommunicationsHandler::UI_SpeedDuel, text);
		_uiEventActive = true;
		_eventStartTime = millis();
		SmartDuelServer::EventName = "Waiting";
	}
}

void SmartDuelEventHandler::HandleOutgoingEvent(String eventData)
{
	String output = _jsonUtility.GetCardEventFromArduino(SocketID, eventData);

#ifdef DEBUG
	Serial.println(output);
#endif // DEBUG


	_server.SendEvent(output);
	_speedDuel.UpdateDuelState(output);
}

String SmartDuelEventHandler::GetTargetZoneName(int multiButtonEvent, int zoneNumber) {
	if (multiButtonEvent == Enums::ButtonClicks::Multi04) return "hand";
	
	String zoneName = "";
	switch (zoneNumber) {
	case 0:
		zoneName = "mainMonster3";
		break;
	case 1:
		zoneName = "mainMonster2";
		break;
	case 2:
		zoneName = "mainMonster1";
		break;
	}

	return zoneName;
}