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
        ButtonEvents[i] = GetButtonEvent(i);
	}

    ButtonEvents[5] = GetMultiButtonEvent();

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

// TODO: Clean up & enable non-trigger button to be released first and still fire multi event
int ButtonHandler::GetMultiButtonEvent() {
    if (ButtonEvents[0] == Enums::ButtonClicks::Hold) {
        if (_buttons[1].IsCurrentlyPressed || _buttons[4].IsCurrentlyPressed) {
            _enableButton1Multi = true;
            _multiEventTriggerTime = millis();
            ButtonEvents[0] = Enums::ButtonClicks::NoChange;
        }        
    }
    else if (ButtonEvents[3] == Enums::ButtonClicks::Hold) {
        if (_buttons[4].IsCurrentlyPressed) {
            _enableButton4Multi = true;
            _multiEventTriggerTime = millis();
            ButtonEvents[3] = Enums::ButtonClicks::NoChange;
        }
    }

    // TODO: Improve Time Expiry
    if ((millis() - _multiEventTriggerTime) > 250 && (_enableButton1Multi || _enableButton4Multi)) {
        _enableButton1Multi = false;
        _enableButton4Multi = false;

        if (_debug) {
            Serial.println("Multi Event Time Expired");
        }
    }
    
    if (_enableButton1Multi) {
        return HandleButton1MultiEvent();
    }
    
    if (_enableButton4Multi) {
        return HandleButton4MultiEvent();
    }

    return Enums::ButtonClicks::NoChange;
}

Enums::ButtonClicks ButtonHandler::HandleButton1MultiEvent() {
    if (ButtonEvents[1] == Enums::ButtonClicks::Hold) {
        _enableButton1Multi = false;
        ButtonEvents[1] = Enums::ButtonClicks::NoChange;
        return Enums::ButtonClicks::Multi12;
    }
    else if (ButtonEvents[4] == Enums::ButtonClicks::Hold) {
        _enableButton1Multi = false;
        ButtonEvents[4] = Enums::ButtonClicks::NoChange;
        return Enums::ButtonClicks::Multi15;
    }

    return Enums::ButtonClicks::NoChange;
}

Enums::ButtonClicks ButtonHandler::HandleButton4MultiEvent() {
    if (ButtonEvents[4] == Enums::ButtonClicks::Hold) {
        _enableButton4Multi = false;
        ButtonEvents[4] = Enums::ButtonClicks::NoChange;
        return Enums::ButtonClicks::Multi45;
    }

    return Enums::ButtonClicks::NoChange;
}

Enums::ButtonClicks ButtonHandler::HandleButtonRelease(Button button) {
    int buttonHoldTime = button.ButtonHoldTime();
    
    if (buttonHoldTime > _holdTime) {
        return Enums::ButtonClicks::Hold;
    }

    if (button.DoubleClickEnabled) {
        return Enums::ButtonClicks::Double;
    }
    
    return Enums::ButtonClicks::EnableDouble;
}

void ButtonHandler::PrintEvents(int buttonEvents[]) {
    for (int i = 0; i < _numButtons + 1; i++) {
        int event = buttonEvents[i];

        if (event != Enums::ButtonClicks::NoChange && i < 5) {
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
        else if (event != Enums::ButtonClicks::NoChange) {
            if (event == Enums::ButtonClicks::Multi12) {
                Serial.println("Multi 1 & 2");
            }
            else if (event == Enums::ButtonClicks::Multi15) {
                Serial.println("Multi 1 & 5");
            }
            else if (event == Enums::ButtonClicks::Multi45) {
                Serial.println("Multi 4 & 5");
            }
        }
    }
}