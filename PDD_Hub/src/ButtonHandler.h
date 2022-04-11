#ifndef ButtonHandler_h
#define ButtonHandler_h
#include "Arduino.h"
#include "Core\Entities\Button.h"
#include "Core\Entities\Enums.h"
#include "PeripheralsHandler.h"

//#define DEBUG_BH

//==============================================//
// MULTI-CLICK: One Button, Multiple EventsStatic
// By Jeff Saltzman Oct. 13, 2009
// 
// Adapted By: SubtleDragon057
// Nov 20, 2021
//==============================================//

class ButtonHandler {
private:
    
    const byte _buttonPin = A0;
    Button _buttons[5] = {
        Button("Button1", _buttonPin),
        Button("Button2", _buttonPin),
        Button("Button3", _buttonPin),
        Button("Button4", _buttonPin),
        Button("Button5", _buttonPin)
    };

    int _debounce;
    int _doubleClickTime;
    int _holdTime;
    int _multiEventTriggerTime;

    int GetButtonEvent(int buttonNum);
    int GetMultiButtonEvent();
    Enums::ButtonClicks HandleMultiButtonEvent();
    Enums::ButtonClicks HandleButtonRelease(Button button);

    PeripheralsHandler* _peripheralsHandler;

    // Debug feature only
    void PrintEvents(int buttonEvents[]);    

public:
    // First 5 Events are single button, the Last is Multi Button
    int ButtonEvents[6];
    
    ButtonHandler(PeripheralsHandler& peripheralsHandler);

    void Initialize(int debouceTime = 20, int doubleClickTime = 275, int holdTime = 1000);
    void CheckButtons();
};

#endif