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
		Connection = 5,
		HasNewEvent = 4,
		GetEventInfo = 3,
		StartDuel = 6,
		EnterWriteMode = 7,
		AcknowledgeSuccess = 9
	};
};

#endif
