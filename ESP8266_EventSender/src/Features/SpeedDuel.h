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
	bool HasAttackTarget = true;
	
	SpeedDuel();

	EventData HandleActivateSpell(String socketID, int zoneNumber);
	EventData HandleActivateMonsterEffect(String socketID, int zoneNumber);
	void HandleAttackEvent(String socketID, int zoneNumber);
	EventData HandleMonsterAttack(String socketID, String zoneName);

	void UpdateDuelistIDs(String socketID, String duelist1, String duelist2);
	void UpdateDuelState(String newDuelState);
	void UpdateDuelState(String duelistID, int cardID, int copyNumber, String zoneName);
	void ClearDuelStates();
};

#endif