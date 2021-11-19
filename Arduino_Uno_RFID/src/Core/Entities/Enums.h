#ifndef Enums_h
#define Enums_h
#include "Arduino.h"

class Enums {
public:
	enum Events {
		NoEvent = 0,
		Summon = 1,
		Remove = 2,
		PositionChange = 3,
		SetSpellTrap = 4,
		ActivateSpellTrap = 5,
	};
	enum CardType {
		Monster,
		Spell
	};
	enum SensorType {
		MonsterAttack,
		MonsterDefence,
		SpellTrap,
		None,
	};
	enum SensorReading {
		Low,
		Medium,
		High
	};
	enum CardPosition {
		FaceUp = 1,
		FaceDown = 2,
		FaceUpDefence = 3,
		FaceDownDefence = 4,
		NoCard = 5
	};
};

#endif