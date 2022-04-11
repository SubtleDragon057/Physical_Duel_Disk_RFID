#ifndef SpeedDuel_h
#define SpeedDuel_h
#include "Arduino.h"
#include "Entities\DuelState.h"
#include "Entities\EventData.h"

class SpeedDuel {
private:
	DuelState _duelState;
	int _attackingMonster;

	bool CheckForValidTarget(String zoneName);

public:
	enum Position {
		FaceUp = 1,
		FaceDown = 2,
		FaceUpDefence = 3,
		FaceDownDefence = 4
	};
	
	SpeedDuel();

	EventData HandleActivateSpell(String socketID, int zoneNumber);
	EventData HandleActivateMonsterEffect(String socketID, int zoneNumber);
	void HandleAttackEvent(String socketID, int zoneNumber);
	EventData HandleMonsterAttack(String socketID, String zoneName);

	void UpdateDuelistIDs(String socketID, String duelist1, String duelist2);
	void UpdateDuelState(String newDuelState);
	void UpdateDuelState(String duelistID, int cardID, int copyNumber, String zoneName, String position);
	void ClearDuelStates();
	void UpdatePhase(String newPhase, bool isOpponentsTurn);
	void UpdateLifepoints(String lifepoints, String duelistID);
	String GetPlayerLP();
	String GetOppLP();
	String GetPhase();
	String HandleChangePhase();
	String GetZoneName(int zone);
};

#endif