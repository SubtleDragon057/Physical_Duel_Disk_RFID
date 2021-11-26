#include "SmartDuelEventHandler.h"
#include "Arduino.h"
#include "Features\Lobby.h"

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

	SendEvent(data);
}

void SmartDuelEventHandler::HandleDuelRoom(int buttonEvents[]) {
	String data = _duelRoom.CheckDuelRoomForAction(buttonEvents);
	if (data == "NoAction") return;

	SendEvent(data);
}

void SmartDuelEventHandler::Connect(String socketIP, int socketPort) {
	_server.Initialize(socketIP, socketPort);
}

String SmartDuelEventHandler::GetSocketId() {
	return _server.GetSocketId();
}

void SmartDuelEventHandler::SendEvent(String eventData)
{
	_server.SendEvent(eventData);
}

void SmartDuelEventHandler::ListenToServer() {
	_server.ListenToServer();
	HandleIncomingEvents();
}

void SmartDuelEventHandler::HandleIncomingEvents() {		
	
	if (SmartDuelServer::ReturnEventName == "room:create") {
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
		IsDueling = true;
		SmartDuelServer::ReturnEventName = "Waiting";
	}
}