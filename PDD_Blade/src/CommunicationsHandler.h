#ifndef CommunicationsHandler_h
#define CommunicationsHandler_h
#include "Arduino.h"
#include "Wire.h"
#include "Core\Entities\Enums.h"

class CommunicationsHandler {
private:
	byte _recievedData;
	const byte _espInterrupt = 8;
	char* _newEventData;

public:
	bool IsInDuel = false;
	bool EnableWriteMode = false;
	String IncomingCardID;
	
	CommunicationsHandler();

	void HandleRecieve();
	void HandleNewEvent(String eventData);
	void GetNextCard();
	void HandleRequest();
};

#endif