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
	enum SingleCardZoneType {
		MonsterZone,
		SpellTrapZone
	};
	enum CardType {
		Monster,
		Spell
	};
	enum SensorReading {
		Low = 0,
		Medium = 2,
		High = 1
	};
	enum CardPosition {
		FaceUp = 1,
		FaceDown = 2,
		FaceUpDefence = 3,
		FaceDownDefence = 4,
		NoCard = 5
	};
	enum Communication {
		CommunicationFailure = 2,
		GetEventInfo = 3,
		EndDuel = 4,
		Connection = 5,
		StartDuel = 6,
		EnterWriteMode = 7,
		ClearEventData = 8,
		AcknowledgeSuccess = 9
	};
};

#endif