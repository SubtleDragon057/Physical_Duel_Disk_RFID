#include "ButtonHandler.h"

#define DEBUG_BH

//==============================================//
// MULTI-CLICK: One Button, Multiple Events
// By Jeff Saltzman Oct. 13, 2009
// 
// Adapted By: SubtleDragon057
// Nov 20, 2021
//==============================================//

ButtonHandler::ButtonHandler() 
{
    for (byte i = 0; i < 5; i++) {
        _buttons[i].Initialize();
    }
}

void ButtonHandler::Initialize(int debouceTime, int doubleClickTime, int holdTime)
{
    _debounce = debouceTime;
    _doubleClickTime = doubleClickTime;
    _holdTime = holdTime;
}

void ButtonHandler::CheckButtons() {
    
    for (byte i = 0; i < 5; i++) {
        ButtonEvents[i] = GetButtonEvent(i);
	}

    ButtonEvents[5] = GetMultiButtonEvent();
    
#ifdef DEBUG_BH
    PrintEvents(ButtonEvents);
#endif // DEBUG_BH
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

int ButtonHandler::GetMultiButtonEvent() {

    for (byte i = 0; i < 5; i++) {
        if (ButtonEvents[i] != Enums::ButtonClicks::Hold) continue;

        _buttons[i].EnableMultiClick = true;
        _multiEventTriggerTime = millis();
        ButtonEvents[i] = Enums::ButtonClicks::NoChange;
    }

    if ((millis() - _multiEventTriggerTime) > 250) {
        for (byte i = 0; i < 5; i++) {
            if (!_buttons[i].EnableMultiClick) continue;

            _buttons[i].EnableMultiClick = false;
            ButtonEvents[i] = Enums::ButtonClicks::Hold;
        }
    }
    
    return HandleMultiButtonEvent();
}

Enums::ButtonClicks ButtonHandler::HandleMultiButtonEvent() {
    bool multiEvents[5];
    memset(multiEvents, false, 5);
    for (byte i = 0; i < 5; i++) {
        multiEvents[i] = _buttons[i].EnableMultiClick;
    }

    if (multiEvents[0] && multiEvents[1]) {
        _buttons[0].EnableMultiClick = false;
        _buttons[1].EnableMultiClick = false;
        return Enums::ButtonClicks::Multi01;
    }
    else if (multiEvents[0] && multiEvents[4]) {
        _buttons[0].EnableMultiClick = false;
        _buttons[4].EnableMultiClick = false;
        return Enums::ButtonClicks::Multi04;
    }
    else if (multiEvents[1] && multiEvents[3]) {
        _buttons[1].EnableMultiClick = false;
        _buttons[3].EnableMultiClick = false;
        return Enums::ButtonClicks::Multi13;
    }
    else if (multiEvents[3] && multiEvents[4]) {
        _buttons[3].EnableMultiClick = false;
        _buttons[4].EnableMultiClick = false;
        return Enums::ButtonClicks::Multi34;
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
    for (byte i = 0; i < 5 + 1; i++) {
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
            if (event == Enums::ButtonClicks::Multi01) {
                Serial.println("Multi 1 & 2");
            }
            else if (event == Enums::ButtonClicks::Multi04) {
                Serial.println("Multi 1 & 5");
            }
            else if (event == Enums::ButtonClicks::Multi34) {
                Serial.println("Multi 4 & 5");
            }
            else if (event == Enums::ButtonClicks::Multi13) {
                Serial.println("Multi 2 & 4");
            }
        }
    }
}