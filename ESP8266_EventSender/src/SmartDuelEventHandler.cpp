#include "SmartDuelEventHandler.h"
#include "Arduino.h"
#include "Features\Lobby.h"
#include "Core\Entities\Enums.h"
#include "Core\Utils\JSONUtility.h"
#include "Features\Entities\EventData.h"

SmartDuelEventHandler::SmartDuelEventHandler(bool debug) {
	_debug = debug;
}

void SmartDuelEventHandler::HandleLobby(int buttonEvents[], int deckList[])
{
	String data = _lobby.CheckLobbyForAction(buttonEvents, deckList);
	if (data == "NoAction") return;

	_server.SendEvent(data);
}

void SmartDuelEventHandler::HandleDuelRoom(int buttonEvents[]) {
	String data = _duelRoom.CheckDuelRoomForAction(buttonEvents);
	if (data == "NoAction") return;

	_server.SendEvent(data);
}

void SmartDuelEventHandler::HandleButtonInteraction(int buttonEvents[], bool isInBattle) {
	EventData newData;
	
	for (int i = 0; i < 6; i++) {
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
			case Enums::ButtonClicks::Multi12:
			case Enums::ButtonClicks::Multi45:
			case Enums::ButtonClicks::Multi15:
				HandleMultiButtonEvent(buttonEvents[i]);
				continue;
		}
	}

	String eventData = _jsonUtility.GetCardEventAsJSON(SocketID, newData.EventName, 
		newData.CardID, newData.CopyNumber, newData.ZoneName);
	_server.SendEvent(eventData);
}

void SmartDuelEventHandler::HandleMultiButtonEvent(int buttonEventType) {
	String eventName;

	switch (buttonEventType) {
		case Enums::ButtonClicks::Multi12:
			eventName = "duelist:flip-coin";
			break;
		case Enums::ButtonClicks::Multi45:
			eventName = "duelist:roll-dice";
			break;
		default:
			eventName = "duelist:flip-coin";
			break;
	}
	
	String eventData = _jsonUtility.GetDuelistEventAsJSON(SocketID, eventName);
	_server.SendEvent(eventData);
}

void SmartDuelEventHandler::Connect(String socketIP, int socketPort) {
	_server.Initialize(socketIP, socketPort);
}

void SmartDuelEventHandler::ListenToServer() {
	String socketID = _server.GetSocketId();
	if (_server.isConnected && (SocketID == NULL || SocketID != socketID)) {
		SocketID = socketID;
		Serial.println("Socket ID: " + SocketID);
	}
	
	_server.ListenToServer();
	HandleIncomingRoomEvents();
	HandleIncomingCardEvents();
}

void SmartDuelEventHandler::HandleIncomingRoomEvents() {		
	
	if (SmartDuelServer::ReturnEventName == "room:create" ||
		SmartDuelServer::ReturnEventName == "room:join") {
		_lobby.UpdateCurrentRoom(SmartDuelServer::ReturnData);
		IsInDuelRoom = true;
		SmartDuelServer::ReturnEventName = "Waiting";
	}	
	else if (SmartDuelServer::ReturnEventName == "room:close") {
		IsInDuelRoom = false;
		IsDueling = false;
		_speedDuel.ClearDuelStates();
		SmartDuelServer::ReturnEventName = "Waiting";
	}
	else if (SmartDuelServer::ReturnEventName == "room:start") {
		_lobby.UpdateCurrentRoom(SmartDuelServer::ReturnData);
		IsInDuelRoom = true;
		_speedDuel.UpdateDuelistIDs(
			SocketID,
			SmartDuelServer::DuelistID1,
			SmartDuelServer::DuelistID2);
		IsDueling = true;
		SmartDuelServer::ReturnEventName = "Waiting";
	}
}

void SmartDuelEventHandler::HandleIncomingCardEvents() {
	if (SmartDuelServer::ReturnEventName == "card:play") {
		_speedDuel.UpdateDuelState(
			SmartDuelServer::DuelistID1,
			SmartDuelServer::CardID,
			SmartDuelServer::CopyNumber,
			SmartDuelServer::ZoneName);
		SmartDuelServer::ReturnEventName = "Waiting";
	}
}

void SmartDuelEventHandler::HandleOutgoingEvent(String eventData)
{
	String output = _jsonUtility.GetCardEventFromArduino(SocketID, eventData);

	if (_debug) {
		Serial.println(output);
	}

	_server.SendEvent(output);
	_speedDuel.UpdateDuelState(output);
}

String SmartDuelEventHandler::GetTargetZoneName(int multiButtonEvent, int zoneNumber) {
	if (multiButtonEvent == Enums::ButtonClicks::Multi15) return "hand";
	
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