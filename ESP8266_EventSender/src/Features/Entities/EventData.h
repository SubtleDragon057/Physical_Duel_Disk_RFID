#ifndef EventData_h
#define EventData_h
#include "Arduino.h"

class EventData {
public:	
	String EventName = "";
	int CardID;
	int CopyNumber;
	String ZoneName;

	EventData() {}
	EventData(String eventName, int cardID, int copyNumber, String zoneName = "") {
		EventName = eventName;
		CardID = cardID;
		CopyNumber = copyNumber;
		ZoneName = zoneName;
	}
};

#endif