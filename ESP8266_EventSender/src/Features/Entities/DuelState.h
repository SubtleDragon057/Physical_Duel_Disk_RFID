#ifndef DuelState_h
#define DuelState_h
#include "Arduino.h"
#include "ArduinoJson.h"
#include "Playerstate.h"

class DuelState {
private:
	PlayerState _playerStates[2] = {
		PlayerState(),
		PlayerState()
	};

	int GetIntValue(String stringToChange);

public:
	DuelState();
	int GetCardID(String duelistID, int zoneNumber, bool isMonsterZone);
	int GetCardID(String duelistID, String zoneName);
	int GetCopyNumber(String duelistID, int zoneNumber, bool isMonsterZone);
	void UpdateDuelistIDs(String socketID, String duelist1, String duelist2);
	void UpdateDuelState(String eventData);
	void UpdateDuelState(String duelistID, int cardID, int copyNumber, String zoneName);
	void ClearPlayerStates();
};

#endif