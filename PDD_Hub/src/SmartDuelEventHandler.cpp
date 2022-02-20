#include "SmartDuelEventHandler.h"
#include "Core\Entities\Enums.h"
#include "Features\Entities\EventData.h"

//#define DEBUG_SDEH

SmartDuelEventHandler::SmartDuelEventHandler(PeripheralsHandler& peripheralsHandler, StorageHandler& storageHandler)
{
	_peripheralsHandler = &peripheralsHandler;
	_storageHandler = &storageHandler;
}

void SmartDuelEventHandler::HandleDuelRoomState(int buttonEvents[]) {
	switch (DuelRoomState) {
		case Enums::Lobby:
			HandleLobby(buttonEvents);
			break;
		case Enums::WaitingForOpponent:
			HandleDuelRoom(buttonEvents);
			break;
		case Enums::IsInDuel:
			HandleButtonInteraction(buttonEvents);
			break;
		case Enums::WaitingForAttackTarget:
			HandleButtonInteraction(buttonEvents, true);
			break;
	}
}

void SmartDuelEventHandler::HandleLobby(int buttonEvents[]) {
#ifdef DEBUG_SDEH
	//Serial.printf("HandleLobby()\n");
#endif // DEBUG_SDEH
	
	DisplayMessageOnScreen(PeripheralsHandler::UI_Lobby, "");

	String data;
	if (buttonEvents[0] != 0) {
		data = _jsonUtility.HandleCreateRoomEvent(_storageHandler->DeckList);
	}
	else if (buttonEvents[1] != 0) {
		data = _jsonUtility.HandleJoinRoomEvent(_storageHandler->DeckList);
	}
	// TODO: Update write mode after adding "Quick-Play Zone"
	//else if (buttonEvents[5] == 9) { // Multi34 Button Event (ie. Hold Buttons 4 & 5)
	//    EnterWriteMode(deckList);
	//	  return;
	//}

	if (data == "") return;

	_server.SendEvent(data);
}

// TODO: This function will lock up the system for its duration
void SmartDuelEventHandler::EnterWriteMode(int deckList[]) {
	String text[] = { "Write Mode" };
	_peripheralsHandler->Display(PeripheralsHandler::UI_WriteMode, text);

	delay(1000);

	Serial.println("Decklist With Copy Num:");
	String deckListWithCopyNum[36];
	for (byte i = 0; i < 36; i++) {
		if (deckList[i] == 0) continue;
		int copyNum = 0;

		for (byte j = 0; j < 36; j++) {
			if (deckList[i] != deckList[j]) continue;
			copyNum++;
		}

		deckListWithCopyNum[i] = String(copyNum) + String(deckList[i]);
		deckList[i] = 0;
		Serial.println(deckListWithCopyNum[i]);
	}

	_peripheralsHandler->EnableWriteMode();
	Serial.printf("Sending Deck\n");

	for (byte i = 0; i < 36; i++) {
		if (deckListWithCopyNum[i] == 0) continue;
		
		String cardNumber[] = { deckListWithCopyNum[i].substring(1) };
		_peripheralsHandler->Display(PeripheralsHandler::UI_WriteMode, cardNumber);
		_peripheralsHandler->TransmitCard(deckListWithCopyNum[i]);
	}

	text[1] = "Done! Returning to Lobby";
	_peripheralsHandler->Display(PeripheralsHandler::UI_WriteMode, text);
	delay(1000);
}

void SmartDuelEventHandler::HandleDuelRoom(int buttonEvents[]) {
#ifdef DEBUG_SDEH
	//Serial.printf("HandleDuelRoom()\n");
#endif // DEBUG_SDEH
	
	DisplayMessageOnScreen(PeripheralsHandler::UI_Lobby, SmartDuelServer::RoomName);
	
	if (buttonEvents[0] == Enums::ButtonClicks::NoChange) return;

	String data = _jsonUtility.HandleCloseRoomEvent(SmartDuelServer::RoomName);
	_server.SendEvent(data);
}

void SmartDuelEventHandler::HandleButtonInteraction(int buttonEvents[], bool isInBattle) {
	EventData newData;
	
	for (byte i = 0; i < _numButtonEvents; i++) {
		if (buttonEvents[i] == Enums::ButtonClicks::NoChange) continue;

		if (isInBattle) {
			String zoneName = GetTargetZoneName(buttonEvents[5], i);
			newData = _speedDuel.HandleMonsterAttack(SocketID, zoneName);
			DuelRoomState = Enums::IsInDuel;
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
				DuelRoomState = Enums::WaitingForAttackTarget;
				_uiEventActive = true;
				DisplayMessageOnScreen(PeripheralsHandler::UI_SpeedDuel, "Choose Target!");
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
		newData.CardID, newData.CopyNumber, newData.ZoneName, newData.Position);
	_server.SendEvent(eventData);

#ifdef DEBUG_SDEH
	Serial.println("HandleButtonInteraction()");
	Serial.println(eventData);
#endif // DEBUG_SDEH

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

void SmartDuelEventHandler::Connect(bool useEncryption) {

	for (uint8_t t = 3; t > 0; t--) {
		DisplayMessageOnScreen(PeripheralsHandler::UI_Init, 
			"[SETUP] Duel Online?  " + String(t));
		delay(1000);
	}

	switch (digitalRead(33)) {
		case LOW:
			DisplayMessageOnScreen(PeripheralsHandler::UI_Init, "[SETUP] Locating CC Servers");
			_server.Initialize(_secrets.onlineServerAddress, _secrets.onlineServerPort);
			break;
		case HIGH:
			DisplayMessageOnScreen(PeripheralsHandler::UI_Init, "[SETUP] Locating Local Server");
			_server.Initialize(_secrets.socketIP, _secrets.socketPort);
			break;
	}
}

void SmartDuelEventHandler::ListenToServer() {
	String socketID = _server.GetSocketId();
	if (_server.isConnected && (SocketID == NULL || SocketID != socketID)) {
		SocketID = socketID;
	}

	_server.ListenToServer();

	if (!_server.isConnected) {
		DisplayMessageOnScreen(PeripheralsHandler::UI_ConnectionError, 
			"No Connection to Server!");
		SmartDuelServer::EventScope = Enums::EventScope::Room;
		SmartDuelServer::EventAction = Enums::EventAction::Close;
		return;
	}

	switch (SmartDuelServer::EventScope) {
		case Enums::EventScope::Room:
			HandleIncomingRoomEvents();
			break;
		case Enums::EventScope::Card:
			HandleIncomingCardEvents();
			break;
		case Enums::EventScope::Duelist:
			HandleIncomingDuelistEvents();
			break;
	}

	if (_uiEventActive && (millis() - _eventStartTime) > 4000) {
		_uiEventActive = false;
		DisplayMessageOnScreen(PeripheralsHandler::UI_SpeedDuel, _speedDuel.GetPhase());
	}

	SmartDuelServer::EventScope = Enums::EventScope::Waiting;
}

void SmartDuelEventHandler::HandleIncomingRoomEvents() {	
	bool isOpponentsTurn = SmartDuelServer::EventData != SocketID;
	
	switch (SmartDuelServer::EventAction) {
		case Enums::EventAction::Create:
		case Enums::EventAction::Join:
			DuelRoomState = Enums::WaitingForOpponent;
			break;
		case Enums::EventAction::Close:
			DuelRoomState = Enums::Lobby;
			_speedDuel.ClearDuelStates();
			_peripheralsHandler->EndDuel();
			_storageHandler->IsDeckSet = false;
			break;
		case Enums::EventAction::Start:
			_speedDuel.UpdateDuelistIDs(
				SocketID,
				SmartDuelServer::DuelistID,
				SmartDuelServer::EventData);
			DuelRoomState = Enums::IsInDuel;
			_uiEventActive = true; // TODO: Puts a 4 second timer in play before showing duel
			_eventStartTime = millis();
			_speedDuel.UpdatePhase("drawPhase", isOpponentsTurn);
			_peripheralsHandler->StartDuelDisk();
			break;
	}

	SmartDuelServer::EventAction = Enums::EventAction::NoCurrentAction;
}

void SmartDuelEventHandler::HandleIncomingCardEvents() {	
	_speedDuel.UpdateDuelState(
		SmartDuelServer::DuelistID,
		SmartDuelServer::CardID,
		SmartDuelServer::CopyNumber,
		SmartDuelServer::EventData,
		SmartDuelServer::Position);
	
	SmartDuelServer::EventAction = Enums::EventAction::NoCurrentAction;
}

void SmartDuelEventHandler::HandleIncomingDuelistEvents() {	
	bool isOpponentsTurn = SmartDuelServer::DuelistID != SocketID;
	switch (SmartDuelServer::EventAction) {
		case Enums::EventAction::Coin:
			DisplayMessageOnScreen(PeripheralsHandler::UI_SpeedDuel, "Flip: " + SmartDuelServer::EventData);
			_uiEventActive = true;
			_eventStartTime = millis();
			break;
		case Enums::EventAction::Dice:
			DisplayMessageOnScreen(PeripheralsHandler::UI_SpeedDuel, "Roll: " + SmartDuelServer::EventData);
			_uiEventActive = true;
			_eventStartTime = millis();
			break;
		case Enums::EventAction::EndTurn:
			_speedDuel.UpdatePhase("drawPhase", !isOpponentsTurn);
			DisplayMessageOnScreen(PeripheralsHandler::UI_SpeedDuel, _speedDuel.GetPhase());
			break;
		case Enums::EventAction::LifePoints:
			_speedDuel.UpdateLifepoints(SmartDuelServer::EventData, SmartDuelServer::DuelistID);
			DisplayMessageOnScreen(PeripheralsHandler::UI_SpeedDuel, _speedDuel.GetPhase());
			break;
		case Enums::EventAction::Phase:
			_speedDuel.UpdatePhase(SmartDuelServer::EventData, isOpponentsTurn);
			DisplayMessageOnScreen(PeripheralsHandler::UI_SpeedDuel, _speedDuel.GetPhase());
			break;
	}

	SmartDuelServer::EventAction = Enums::EventAction::NoCurrentAction;
}

void SmartDuelEventHandler::HandleOutgoingEvent(String eventData) {
	String output = _jsonUtility.GetCardEventFromArduino(SocketID, eventData);

#ifdef DEBUG_SDEH
	Serial.println("HandleOutgoingEvent()");
	Serial.println(output);
#endif // DEBUG

	_server.SendEvent(output);
	_speedDuel.UpdateDuelState(output);
}

void SmartDuelEventHandler::DisplayMessageOnScreen(PeripheralsHandler::UI_Type uiType, String message) {
	String data[3] = { message, _speedDuel.GetPlayerLP(), _speedDuel.GetOppLP() };
	_peripheralsHandler->Display(uiType, data);
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