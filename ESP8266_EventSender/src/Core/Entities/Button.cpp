#include "Button.h"
#include "Arduino.h"
#include "Enums.h"

Button::Button() 
{
}
Button::Button(String name, byte pin) {
	_name = name;
	_pin = pin;
}

void Button::Initialize() {
	pinMode(_pin, INPUT_PULLUP);

    _currentState = Enums::ButtonState::ReleasedState;
    _previousState = Enums::ButtonState::ReleasedState;
}

Enums::ButtonClicks Button::CheckButtonForEvent(int debounceTime, int doubleClickTime) {
    Enums::ButtonClicks event = Enums::ButtonClicks::NoChange;
    _currentState = digitalRead(_pin);
    _currentMillis = millis();
    
    if (DoubleClickExpired(doubleClickTime)) {
        enableDoubleClick = false;
        event = Enums::ButtonClicks::Single;
    }
    
    if (ButtonIsPressed(debounceTime)) {
        _timeSincePressed = _currentMillis;
        _previousState = _currentState;

        if (enableDoubleClick && (_currentMillis - _timeSinceReleased) < doubleClickTime) { // Doesn't account for button hold time
            _dcOnRelease = true;
        }
        else {
            _dcOnRelease = false;
        }

        event = Enums::ButtonClicks::ButtonPressed;
    }
    
    if (ButtonIsReleased(debounceTime)) {
        _timeSinceReleased = _currentMillis;
        _previousState = _currentState;
        event = Enums::ButtonClicks::ButtonReleased;

        if (_dcOnRelease) {
            _dcOnRelease = false;
            enableDoubleClick = false;
            event = Enums::ButtonClicks::Double;
        }        
    }
    
    return event;
}

bool Button::ButtonIsPressed(int debounceTime) {
    if ((_currentMillis - _timeSinceReleased) < debounceTime) {
        return false;
    }
    
    return _currentState == Enums::PressedState && _previousState == Enums::ReleasedState;
}

bool Button::ButtonIsReleased(int debounceTime) {
    if ((_currentMillis - _timeSincePressed) < debounceTime) {
        return false;
    }

    return _currentState == Enums::ReleasedState && _previousState == Enums::PressedState;
}

bool Button::DoubleClickExpired(int doubleClickTime) {
    return (_currentMillis - _timeSinceReleased) > doubleClickTime && enableDoubleClick && !_dcOnRelease;
}

void Button::EnableDoubleClick() {
    enableDoubleClick = true;
}