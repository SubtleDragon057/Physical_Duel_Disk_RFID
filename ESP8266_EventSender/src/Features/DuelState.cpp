#include "DuelState.h"
#include "Arduino.h"
#include "ArduinoJson.h"
#include "Entities\Playerstate.h"

DuelState::DuelState()
{
}

int DuelState::GetCardID(String duelistID, int zoneNumber, bool isMonsterZone) {
	Playerstate currentPlayer = _playerstates[0].DuelistID() == duelistID 
		? _playerstates[0] 
		: _playerstates[1];

	if (currentPlayer.IsZoneEmpty(isMonsterZone, zoneNumber)) return 0;

	return currentPlayer.GetCardID(isMonsterZone, zoneNumber);
}

int DuelState::GetCopyNumber(String duelistID, int zoneNumber, bool isMonsterZone) {
	Playerstate currentPlayer = _playerstates[0].DuelistID() == duelistID
		? _playerstates[0]
		: _playerstates[1];

	if (currentPlayer.IsZoneEmpty(isMonsterZone, zoneNumber)) return 0;

	return currentPlayer.GetCopyNumber(isMonsterZone, zoneNumber);
}

void DuelState::UpdateDuelistIDs(String socketID, String duelist1, String duelist2) {
	String opponentID = socketID == duelist1 ? duelist2 : duelist1;

	_playerstates[0].UpdateDuelistID(socketID, false);
	_playerstates[1].UpdateDuelistID(opponentID, true);
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
		if (_playerstates[i].DuelistID() != duelistID) continue;
		_playerstates[i].UpdatePlayerstate(GetIntValue(cardID), GetIntValue(copyNum), zoneName);
	}
}
void DuelState::UpdateDuelState(String duelistID, int cardID, int copyNumber, String zoneName) {
	for (int i = 0; i < 2; i++) {
		if (_playerstates[i].DuelistID() != duelistID) continue;
		_playerstates[i].UpdatePlayerstate(cardID, copyNumber, zoneName);
	}
}

int DuelState::GetIntValue(String stringToChange) {
	char charArray[9];
	stringToChange.toCharArray(charArray, 9);

	return atoi(&charArray[0]);
}