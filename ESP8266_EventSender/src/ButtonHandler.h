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
	Button _buttons[5];

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

    void Initialize(Button buttons[], int debouceTime = 20, int doubleClickTime = 275, int holdTime = 1000);
    void CheckButtons();
};

#endif