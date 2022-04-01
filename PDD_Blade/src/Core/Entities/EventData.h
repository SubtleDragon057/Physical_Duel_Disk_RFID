#ifndef EventData_h
#define EventData_h
#include "Arduino.h"

class EventData {
public:
	char* Data;
	
	EventData();

	void WriteEventData(String eventData);
};

#endif