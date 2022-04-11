#ifndef DuelState_h
#define DuelState_h
#include "Arduino.h"
#include "Playerstate.h"

class DuelState {
private:	
	PlayerState _playerStates[2] = {
		PlayerState(),
		PlayerState()
	};

	int GetIntValue(String stringToChange);
	String GetShortenedPhaseName(String phase);

public:
	String CurrentPhase;
	bool IsOpponentsTurn;

	DuelState();

	int GetCardID(String duelistID, int zoneNumber, bool isMonsterZone);
	int GetCardID(String duelistID, String zoneName);
	int GetCopyNumber(String duelistID, int zoneNumber, bool isMonsterZone);
	String GetCardPosition(String duelistID, int zoneNumber, bool isMonsterZone);
	void UpdateDuelistIDs(String socketID, String duelist1, String duelist2);
	void UpdateDuelState(String eventData);
	void UpdateDuelState(String duelistID, int cardID, int copyNumber, String zoneName, String position);
	void UpdatePhase(String newPhase, bool isOpponentsTurn = false);
	void UpdateLifepoints(int lifepoints, String duelistID);
	int GetPlayerLP();
	int GetOppLP();
	String GetPhase();
	void ClearPlayerStates();
};

#endif