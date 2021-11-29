#ifndef CommunicationsHandler_h
#define CommunicationsHandler_h
#include "Arduino.h"
#include "Wire.h"
#include "Core\Entities\Enums.h"

class CommunicationsHandler {
private:
	byte _recievedData;
	bool _hasNewEvent = false;
	char _newEventData[12];

	bool _debug;
public:
	CommunicationsHandler(bool debug);

	void HandleRecieve();
	void HandleNewEvent(String eventData);
	void HandleRequest();
};

#endif