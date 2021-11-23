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

    bool _debug;

    int GetButtonEvent(int buttonNum);
    void PrintEvents(int buttonEvents[]);
    Enums::ButtonClicks HandleButtonRelease(Button button);
    

public:
    int ButtonEvents[5];
    
    ButtonHandler(bool debug);

	void Initialize(byte numButtons, Button buttons[]);
    void Initialize(byte numButtons, Button buttons[], int debouceTime, int doubleClickTime, int holdTime);    
    void CheckButtons();
};

#endif