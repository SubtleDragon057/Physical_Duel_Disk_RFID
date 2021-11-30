#include "SmartDuelEventHandler.h"
#include "Arduino.h"
#include "Features\Lobby.h"
#include "Core\Entities\Enums.h"

SmartDuelEventHandler::SmartDuelEventHandler(bool debug) {
	_debug = debug;
}

void SmartDuelEventHandler::InitializeLobby(int deckList[]) {
	_lobby.Initialize(deckList);
}

void SmartDuelEventHandler::HandleLobby(int buttonEvents[])
{
	String data = _lobby.CheckLobbyForAction(buttonEvents);
	if (data == "NoAction") return;

	_server.SendEvent(data);
}

void SmartDuelEventHandler::HandleDuelRoom(int buttonEvents[]) {
	String data = _duelRoom.CheckDuelRoomForAction(buttonEvents);
	if (data == "NoAction") return;

	_server.SendEvent(data);
}

void SmartDuelEventHandler::HandleButtonInteraction(int buttonEvents[]) {
	for (int i = 0; i < 5; i++) {
		if (buttonEvents[i] == Enums::ButtonClicks::NoChange) continue;

		if(buttonEvents[i] == Enums::ButtonClicks::Single) {
			HandleActivateSpell(i);
		}
		else if (buttonEvents[i] == Enums::ButtonClicks::Double) {
			HandleActivateMonsterEffect(i);
		}
		else if (buttonEvents[i] == Enums::ButtonClicks::Hold) {
			HandleMonsterAttack(i);
		}
	}
}

void SmartDuelEventHandler::HandleActivateSpell(int zoneNumber) {
	int spellID = _duelState.GetCardID(SocketID, zoneNumber, false);
	int copyNum = _duelState.GetCopyNumber(SocketID, zoneNumber, false);
	if (spellID == 0) {
		Serial.printf("No valid Spell on Zone: %i\n", zoneNumber);
		return;
	}
	
	String eventData = _eventWrapper.GetDeclareEventAsJSON(SocketID, spellID, copyNum);
	_server.SendEvent(eventData);
	Serial.printf("Spell %i activated on zone: %i\n", spellID, zoneNumber);
}

void SmartDuelEventHandler::HandleActivateMonsterEffect(int zoneNumber) {
	int monsterID = _duelState.GetCardID(SocketID, zoneNumber, true);
	int copyNum = _duelState.GetCopyNumber(SocketID, zoneNumber, true);
	if (monsterID == 0) {
		Serial.printf("No valid Spell on Zone: %i\n", zoneNumber);
		return;
	}

	String eventData = _eventWrapper.GetDeclareEventAsJSON(SocketID, monsterID, copyNum);
	_server.SendEvent(eventData);
	Serial.printf("Monster %i activated on zone: %i\n", monsterID, zoneNumber);
}

void SmartDuelEventHandler::HandleMonsterAttack(int zoneNumber) {
	int monsterID = _duelState.GetCardID(SocketID, zoneNumber, true);
	if (monsterID == 0) {
		Serial.printf("No valid Monster on Zone: %i\n", zoneNumber);
		return;
	}

	_attackingMonster = zoneNumber;
	HasAttackTarget = false;
}

void SmartDuelEventHandler::HandleAttackEvent(int buttonEvents[]) {
	for (int i = 0; i < 5; i++) {
		if (buttonEvents[i] == Enums::ButtonClicks::NoChange) continue;
		if (!CheckForValidTarget(i)) {
			Serial.printf("No Targets on Zone: %i\n", i);
			return;
		}

		HasAttackTarget = true;
		int monsterID = _duelState.GetCardID(SocketID, _attackingMonster, true);
		int copyNum = _duelState.GetCopyNumber(SocketID, _attackingMonster, true);
		String zoneName = _eventWrapper.GetTargetZoneName(i);

		String eventData = _eventWrapper.GetAttackEventAsJSON(SocketID, monsterID, copyNum, zoneName);
		_server.SendEvent(eventData);
	}
}

bool SmartDuelEventHandler::CheckForValidTarget(int zoneNumber) {
	int targetID = _duelState.GetCardID("", zoneNumber, true);
	return targetID != 0;
}

void SmartDuelEventHandler::Connect(String socketIP, int socketPort) {
	_server.Initialize(socketIP, socketPort);
}

void SmartDuelEventHandler::ListenToServer() {
	if (_server.isConnected && SocketID == NULL) {
		SocketID = _server.GetSocketId();
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
		SmartDuelServer::ReturnEventName = "Waiting";
	}
	else if (SmartDuelServer::ReturnEventName == "room:start") {
		_lobby.UpdateCurrentRoom(SmartDuelServer::ReturnData);
		IsInDuelRoom = true;
		_duelState.UpdateDuelistIDs(
			SocketID,
			SmartDuelServer::DuelistID1,
			SmartDuelServer::DuelistID2);
		IsDueling = true;
		SmartDuelServer::ReturnEventName = "Waiting";
	}	
}

void SmartDuelEventHandler::HandleIncomingCardEvents() {
	if (SmartDuelServer::ReturnEventName == "card:play") {
		_duelState.UpdateDuelState(
			SmartDuelServer::DuelistID1,
			SmartDuelServer::CardID,
			SmartDuelServer::CopyNumber,
			SmartDuelServer::ZoneName);
		SmartDuelServer::ReturnEventName = "Waiting";
	}
}

void SmartDuelEventHandler::HandleOutgoingEvent(String eventData)
{
	String output = _eventWrapper.GetCardEventAsJSON(SocketID, eventData);

	if (_debug) {
		Serial.println(output);
	}

	_server.SendEvent(output);
	_duelState.UpdateDuelState(output);
}