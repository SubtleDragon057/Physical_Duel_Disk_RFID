#ifndef Enums_h
#define Enums_h
#include "Arduino.h"

class Enums {
public:
	enum ButtonState {
		PressedState = LOW,
		ReleasedState = HIGH
	};
	enum ButtonClicks {
		NoChange,
		ButtonPressed,
		ButtonReleased,
		Single,
		Double,
		EnableDouble,
		Hold,
		Multi01,
		Multi13,
		Multi34,
		Multi04
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
	enum DuelRoomState {
		Lobby,
		WaitingForOpponent,
		IsInDuel,
		WaitingForAttackTarget
	};
	enum EventScope {
		Waiting,
		Room,
		Card,
		Duelist
	};
	enum EventAction {
		NoCurrentAction,
		Create,
		Start,
		Join,
		Close,
		Phase,
		LifePoints,
		Coin,
		Dice,
		EndTurn
	};
};

#endif
