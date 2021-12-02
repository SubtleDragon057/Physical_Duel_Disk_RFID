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
		Multi12,
		Multi45,
		Multi15
	};
	enum Communication {
		Connection = 0x05,
		HasNewEvent = 0x04,
		GetEventInfo = 0x03
	};
};

#endif
