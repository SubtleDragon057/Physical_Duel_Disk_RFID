#ifndef CommunicationsHandler_h
#define CommunicationsHandler_h
#include "Arduino.h"
#include "Wire.h"
#include "Core\Entities\Enums.h"

//#define DEBUG_CH

class CommunicationsHandler {
private:
	byte _recievedData;
	bool _hasNewEvent = false;
	char* _newEventData;

public:
	bool IsInDuel = false;
	bool EnableWriteMode = false;
	String IncomingCardID;
	
	CommunicationsHandler();

	void HandleRecieve();
	void HandleNewEvent(char* eventData);
	void GetNextCard();
	void HandleRequest();
};

#endif