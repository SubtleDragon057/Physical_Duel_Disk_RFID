#ifndef Playerstate_h
#define Playerstate_h
#include "Arduino.h"
#include "Zones.h"

class Playerstate {
private:
	bool _isOppnent;
	String _duelistID;

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

public:
	Playerstate();

	String DuelistID() { return _duelistID; }
	bool IsZoneEmpty(bool isMonsterZone, int zoneNumber);
	int GetCardID(bool isMonsterZone, int zoneNumber);
	int GetCopyNumber(bool isMonsterZone, int zoneNumber);
	void UpdateDuelistID(String duelistID, bool isOpponent);
	void UpdatePlayerstate(int cardID, int copyNumber, String zoneName);
};

#endif