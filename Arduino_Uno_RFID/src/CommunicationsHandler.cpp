#include "CommunicationsHandler.h"
#include "Arduino.h"
#include "Wire.h"
#include "Core\Entities\Enums.h"

CommunicationsHandler::CommunicationsHandler(bool debug)
{
	_debug = debug;
}

void CommunicationsHandler::HandleRecieve() {
	while (Wire.available()) {
		_recievedData = Wire.read();
	}
}

void CommunicationsHandler::HandleRequest() {
	if (_recievedData == Enums::Communication::Connection) {
		Wire.write("057");
	}
	else if (_recievedData == Enums::Communication::HasNewEvent) {
		byte hasChange = _hasNewEvent ? 0x01 : 0x00;
		Wire.write(hasChange);
	}
	else if (_recievedData == Enums::Communication::GetEventInfo) {
		Wire.write(_newEventData);
		_hasNewEvent = false;
		Serial.println(_newEventData);
	}
}

void CommunicationsHandler::HandleNewEvent(String eventData) {
	_hasNewEvent = true;
	eventData.toCharArray(_newEventData, 12);
}