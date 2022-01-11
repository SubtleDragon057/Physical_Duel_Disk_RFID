#include "CommunicationsHandler.h"
#include "Arduino.h"
#include "Wire.h"
#include "Core\Entities\Enums.h"

CommunicationsHandler::CommunicationsHandler()
{
}

void CommunicationsHandler::HandleRecieve() {	
	if (EnableWriteMode) {
		IncomingCardID = "";
		while (Wire.available()) {
			char message = Wire.read();
			IncomingCardID += message;
		}

#ifdef DEBUG_CH
		Serial.print("Incoming ID: "); Serial.println(IncomingCardID);
#endif // DEBUG

		return;
	}
	
	while (Wire.available()) {
		_recievedData = Wire.read();
	}
}

void CommunicationsHandler::HandleRequest() {		
	switch (_recievedData) {
		case Enums::Communication::EnterWriteMode:
			Wire.write("Sub");
			EnableWriteMode = true;
			_recievedData = 0;
			break;
		case Enums::Communication::Connection:
			Wire.write("057");
			_recievedData = 0;
			break;
		case Enums::Communication::StartDuel:
			Wire.write("Dra");
			IsInDuel = true;
			break;
		case Enums::Communication::HasNewEvent:
			Wire.write(_hasNewEvent);
			break;
		case Enums::Communication::GetEventInfo:
			Wire.write(_newEventData);
			_hasNewEvent = false;
			break;
	}
}

void CommunicationsHandler::HandleNewEvent(char* eventData) {
	_hasNewEvent = true;
	_newEventData = eventData;
}

void CommunicationsHandler::GetNextCard() {
	_recievedData = 7;
}