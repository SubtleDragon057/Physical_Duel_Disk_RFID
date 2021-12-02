#include "DuelState.h"
#include "Arduino.h"
#include "ArduinoJson.h"
#include "Playerstate.h"

DuelState::DuelState()
{
}

int DuelState::GetCardID(String duelistID, int zoneNumber, bool isMonsterZone) {
	PlayerState currentPlayer = _playerStates[0].DuelistID() == duelistID 
		? _playerStates[0] 
		: _playerStates[1];

	if (currentPlayer.IsZoneEmpty(isMonsterZone, zoneNumber)) return 0;

	return currentPlayer.GetCardID(isMonsterZone, zoneNumber);
}
int DuelState::GetCardID(String duelistID, String zoneName) {
	PlayerState currentPlayer = _playerStates[0].DuelistID() == duelistID
		? _playerStates[0]
		: _playerStates[1];

	if (currentPlayer.IsZoneEmpty(zoneName)) return 0;

	return currentPlayer.GetCardID(zoneName);
}

int DuelState::GetCopyNumber(String duelistID, int zoneNumber, bool isMonsterZone) {
	PlayerState currentPlayer = _playerStates[0].DuelistID() == duelistID
		? _playerStates[0]
		: _playerStates[1];

	if (currentPlayer.IsZoneEmpty(isMonsterZone, zoneNumber)) return 0;

	return currentPlayer.GetCopyNumber(isMonsterZone, zoneNumber);
}

void DuelState::UpdateDuelistIDs(String socketID, String duelist1, String duelist2) {
	String opponentID = socketID == duelist1 ? duelist2 : duelist1;

	_playerStates[0].UpdateDuelistID(socketID, false);
	_playerStates[1].UpdateDuelistID(opponentID, true);
}

void DuelState::UpdateDuelState(String eventData) {
    DynamicJsonDocument doc(512);
    DeserializationError error = deserializeJson(doc, eventData);
    if (error) {
        Serial.print("Error: ");
        Serial.println(error.c_str());
        return;
    }

	String duelistID = doc[1]["duelistId"];
	String cardID = doc[1]["cardId"];
	String copyNum = doc[1]["copyNumber"];
	String zoneName = doc[1]["zoneName"];

	for (int i = 0; i < 2; i++) {
		if (_playerStates[i].DuelistID() != duelistID) continue;
		_playerStates[i].UpdatePlayerstate(GetIntValue(cardID), GetIntValue(copyNum), zoneName);
	}
}
void DuelState::UpdateDuelState(String duelistID, int cardID, int copyNumber, String zoneName) {
	for (int i = 0; i < 2; i++) {
		if (_playerStates[i].DuelistID() != duelistID) continue;
		_playerStates[i].UpdatePlayerstate(cardID, copyNumber, zoneName);
	}
}

void DuelState::ClearPlayerStates() {
	for (int i = 0; i < 2; i++) {
		_playerStates[i].Clear();
	}
}

int DuelState::GetIntValue(String stringToChange) {
	char charArray[9];
	stringToChange.toCharArray(charArray, 9);

	return atoi(&charArray[0]);
}