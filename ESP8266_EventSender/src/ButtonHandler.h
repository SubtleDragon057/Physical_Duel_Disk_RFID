#ifndef ButtonHandler_h
#define ButtonHandler_h
#include "Arduino.h"
#include "Core\Entities\Button.h"

//==============================================//
// MULTI-CLICK: One Button, Multiple EventsStatic
// By Jeff Saltzman Oct. 13, 2009
// 
// Adapted By: SubtleDragon057
// Nov 20, 2021
//==============================================//

class ButtonHandler {
private:
	byte _numButtons;
	Button _buttons[5];

    int _debounce = 20;
    int _doubleClickTime = 275;
    int _holdTime = 1000;
    int _multiEventTriggerTime;

    bool _enableButton1Multi = false;
    bool _enableButton4Multi = false;

    bool _debug;

    int GetButtonEvent(int buttonNum);
    int GetMultiButtonEvent();
    Enums::ButtonClicks HandleButton1MultiEvent();
    Enums::ButtonClicks HandleButton4MultiEvent();
    Enums::ButtonClicks HandleButtonRelease(Button button);

    // Debug feature only
    void PrintEvents(int buttonEvents[]);    

public:
    // First 5 Events are single button, the Last is Multi Button
    int ButtonEvents[6];
    
    ButtonHandler(bool debug);

	void Initialize(byte numButtons, Button buttons[]);
    void Initialize(byte numButtons, Button buttons[], int debouceTime, int doubleClickTime, int holdTime);    
    void CheckButtons();
};

#endif