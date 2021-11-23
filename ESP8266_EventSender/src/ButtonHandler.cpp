#include "ButtonHandler.h"
#include "Arduino.h"
#include "Core\Entities\Button.h"
#include "Core\Entities\Enums.h"

//==============================================//
// MULTI-CLICK: One Button, Multiple Events
// By Jeff Saltzman Oct. 13, 2009
// 
// Adapted By: SubtleDragon057
// Nov 20, 2021
//==============================================//

ButtonHandler::ButtonHandler(bool debug) 
{
    _debug = debug;
}

void ButtonHandler::Initialize(byte numButtons, Button buttons[]) {
	_numButtons = numButtons;
	
	for (int i = 0; i < _numButtons; i++) {
		_buttons[i] = buttons[i];
		_buttons[i].Initialize();
	}
}
void ButtonHandler::Initialize(byte numButtons, Button buttons[], int debouceTime, int doubleClickTime, int holdTime)
{
    _numButtons = numButtons;

    for (int i = 0; i < _numButtons; i++) {
        _buttons[i] = buttons[i];
        _buttons[i].Initialize();
    }
    
    _debounce = debouceTime;
    _doubleClickTime = doubleClickTime;
    _holdTime = holdTime;
}

void ButtonHandler::CheckButtons() {
    
    for (int i = 0; i < _numButtons; i++) {
        int buttonEvent = GetButtonEvent(i);
        ButtonEvents[i] = buttonEvent;
		delay(25);
	}

    if (_debug) {
        PrintEvents(ButtonEvents);
    }
}

int ButtonHandler::GetButtonEvent(int buttonNum) {

    int event = _buttons[buttonNum].CheckButtonForEvent(_debounce, _doubleClickTime);
    if (event == Enums::ButtonClicks::ButtonPressed ||
        event == Enums::ButtonClicks::NoChange) {
        return Enums::ButtonClicks::NoChange;
    }

    if (event == Enums::ButtonClicks::ButtonReleased) {
        event = HandleButtonRelease(_buttons[buttonNum]);
    }
    
    if (event == Enums::ButtonClicks::EnableDouble) {
        _buttons[buttonNum].EnableDoubleClick();
        event = Enums::ButtonClicks::NoChange;
    }

    return event != Enums::ButtonClicks::ButtonReleased 
        ? event 
        : Enums::ButtonClicks::NoChange;
}

Enums::ButtonClicks ButtonHandler::HandleButtonRelease(Button button) {
    int buttonHoldTime = button.ButtonHoldTime();
    
    if (buttonHoldTime > _holdTime) {
        return Enums::ButtonClicks::Hold;
    }

    if (button.enableDoubleClick) {
        return Enums::ButtonClicks::Double;
    }
    
    return Enums::ButtonClicks::EnableDouble;
}

void ButtonHandler::PrintEvents(int buttonEvents[]) {
    for (int i = 0; i < _numButtons; i++) {
        int event = buttonEvents[i];

        if (event != Enums::ButtonClicks::NoChange) {
            Serial.print(_buttons[i].Name());

            if (event == Enums::ButtonClicks::Single) {
                Serial.println(": Single Click");
            }
            else if (event == Enums::ButtonClicks::Double) {
                Serial.println(": Double Click");
            }
            else if (event == Enums::ButtonClicks::Hold) {
                Serial.println(": Hold");
            }
        }
    }
}