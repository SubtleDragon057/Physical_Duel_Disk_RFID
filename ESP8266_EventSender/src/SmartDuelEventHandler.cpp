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

	_server.SendEvent(data);
}

void SmartDuelEventHandler::HandleDuelRoom(int buttonEvents[]) {
	String data = _duelRoom.CheckDuelRoomForAction(buttonEvents);
	if (data == "NoAction") return;

	_server.SendEvent(data);
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

void SmartDuelEventHandler::HandleOutgoingEvent(String eventData)
{
	String output = _eventWrapper.GetCardEventAsJSON(SocketID, eventData);

	if (_debug) {
		Serial.println(output);
	}

	_server.SendEvent(output);
}
