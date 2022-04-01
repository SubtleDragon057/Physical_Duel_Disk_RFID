#ifndef Playerstate_h
#define Playerstate_h
#include "Arduino.h"
#include "Zones.h"

class PlayerState {
private:
	bool _isOppnent;
	String _duelistID;
	int _lifepoints = 4000;

	SingleCardZone _singleZones[8] = {
		SingleCardZone("mainMonster1"),
		SingleCardZone("mainMonster2"),
		SingleCardZone("mainMonster3"),
		SingleCardZone("spellTrap1"),
		SingleCardZone("spellTrap2"),
		SingleCardZone("spellTrap3"),
		SingleCardZone("field"),
		SingleCardZone("skill")
	};

	MultiCardZone _multiZones[5] = {
		MultiCardZone("deck"),
		MultiCardZone("extraDeck"),
		MultiCardZone("hand"),
		MultiCardZone("graveyard"),
		MultiCardZone("banished")
	};

	void RemoveFromPreviousZone(int cardID, int copyNumber);

public:
	PlayerState();

	bool IsOpponent() { return _isOppnent; }
	String DuelistID() { return _duelistID; }
	int LifePoints() { return _lifepoints; }
	bool IsZoneEmpty(bool isMonsterZone, int zoneNumber);
	bool IsZoneEmpty(String zoneName);
	int GetCardID(bool isMonsterZone, int zoneNumber);
	int GetCardID(String zoneName);
	int GetCopyNumber(bool isMonsterZone, int zoneNumber);
	int GetCopyNumber(String zoneName);
	int GetCardPosition(bool isMonsterZone, int zoneNumber);
	int GetCardPosition(String zoneName);
	void UpdateDuelistID(String duelistID, bool isOpponent);
	void UpdatePlayerstate(int cardID, int copyNumber, String zoneName, int position);
	void UpdateLifepoints(int lifepoints);
	void Clear();
};

#endif