#ifndef ButtonHandler_h
#define ButtonHandler_h
#include "Arduino.h"
#include "Core\Entities\Button.h"
#include "Core\Entities\Enums.h"

//==============================================//
// MULTI-CLICK: One Button, Multiple EventsStatic
// By Jeff Saltzman Oct. 13, 2009
// 
// Adapted By: SubtleDragon057
// Nov 20, 2021
//==============================================//

class ButtonHandler {
private:
#ifdef ESP8266;
    byte button1Pin = 0;
    byte button2Pin = 2;
    byte button3Pin = 14;
    byte button4Pin = 12;
    byte button5Pin = 13;
#endif
#ifdef ESP32
    const byte button1Pin = 32;
    const byte button2Pin = 33;
    const byte button3Pin = 25;
    const byte button4Pin = 26;
    const byte button5Pin = 27;
#endif

    Button _buttons[5] = {
        Button("Button1", button1Pin),
        Button("Button2", button2Pin),
        Button("Button3", button3Pin),
        Button("Button4", button4Pin),
        Button("Button5", button5Pin)
    };

    int _debounce;
    int _doubleClickTime;
    int _holdTime;
    int _multiEventTriggerTime;

    int GetButtonEvent(int buttonNum);
    int GetMultiButtonEvent();
    Enums::ButtonClicks HandleMultiButtonEvent();
    Enums::ButtonClicks HandleButtonRelease(Button button);

    // Debug feature only
    void PrintEvents(int buttonEvents[]);    

public:
    // First 5 Events are single button, the Last is Multi Button
    int ButtonEvents[6];
    
    ButtonHandler();

    void Initialize(int debouceTime = 20, int doubleClickTime = 275, int holdTime = 1000);
    void CheckButtons();
};

#endif