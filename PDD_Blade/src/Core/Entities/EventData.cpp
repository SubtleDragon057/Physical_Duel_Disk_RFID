#include "EventData.h"

#define DEBUG_ED

EventData::EventData() {}

void EventData::WriteEventData(String eventData) {
#ifdef DEBUG_ED
	Serial.print("Overwritting Event Data with: "); Serial.println(eventData);
#endif

	char buffer[13];
	eventData.toCharArray(buffer, 13);
	Data = buffer;

#ifdef DEBUG_ED
	Serial.print("New Event Data: ");
	for (byte i = 0; i < 13; i++) {
		Serial.print(Data[i]);
	}
	Serial.println();
#endif
}
