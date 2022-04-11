#ifndef Button_h
#define Button_h
#include "Arduino.h"
#include "Enums.h"

class Button {
private:
	String _name;
	byte _pin;
	int _currentState;
	int _previousState;
	long _timeSincePressed = 0;
	long _timeSinceReleased = 0;
	long _currentMillis;
	bool _dcOnRelease = false;

	bool ButtonIsPressed(int debounceTime);
	bool ButtonIsReleased(int debounceTime);
	bool DoubleClickExpired(int doubleClickTime);

public:	
	bool IsCurrentlyPressed = false;
	bool DoubleClickEnabled = false;
	bool EnableMultiClick = false;
	
	Button(String name, byte pin);

	String Name() { return _name; }
	int ButtonHoldTime() { return _timeSinceReleased - _timeSincePressed; }

	Enums::ButtonClicks CheckButtonForEvent(int debounce, int doubleClickTime);
	void EnableDoubleClick();
};

#endif