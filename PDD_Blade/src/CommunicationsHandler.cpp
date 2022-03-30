#include "CommunicationsHandler.h"
#include "Arduino.h"
#include "Wire.h"
#include "Core\Entities\Enums.h"

#define DEBUG_CH

CommunicationsHandler::CommunicationsHandler() {
	pinMode(_espInterrupt, OUTPUT);
	digitalWrite(_espInterrupt, HIGH);
}

void CommunicationsHandler::HandleRecieve() {	
#ifdef DEBUG_CH
	Serial.println("Handle Receive");
#endif // DEBUG_CH
	
	if (EnableWriteMode) {
		IncomingCardID = "";
		while (Wire.available()) {
			char message = Wire.read();
			IncomingCardID += message;
		}

#ifdef DEBUG_CH
		Serial.print("Incoming ID: "); Serial.print(IncomingCardID);
		Serial.println("~");
#endif // DEBUG

		return;
	}
	
	while (Wire.available()) {
		_recievedData = Wire.read();
	}

	switch (_recievedData) {
		case Enums::Communication::EndDuel:
			IsInDuel = false;
#ifdef DEBUG_CH
			Serial.println("The Duel Has Ended!");
#endif // DEBUG_CH
			break;
		case Enums::Communication::ClearEventData:
			SendEventSuccess = true;
			_newEventData = "";
#ifdef DEBUG_CH
			Serial.println("Event Recieved - Clearing Data");
#endif // DEBUG_CH
			break;
		case Enums::Communication::CommunicationFailure:
			// Handle Failure
#ifdef DEBUG_CH
			Serial.println("Communication Failure!");
			Serial.print("Failed Data: ");
			for (byte i = 0; i < 13; i++) {
				Serial.print(_newEventData[i]);
			}
			Serial.println();
#endif // DEBUG_CH
			break;
	}
}

void CommunicationsHandler::HandleRequest() {		
#ifdef DEBUG_CH
	Serial.println("Handle Request");
#endif // DEBUG_CH
	
	if (IsInDuel) {
		Wire.write(GetEventData());
		return;
	}
	
	switch (_recievedData) {
		case Enums::Communication::EnterWriteMode:
			Wire.write("Sub");
			EnableWriteMode = true;
			_recievedData = 0;
			break;
		case Enums::Communication::Connection:
			Wire.write("057");
			_recievedData = 0;

			digitalWrite(_espInterrupt, LOW);
			delay(10);
			digitalWrite(_espInterrupt, HIGH);
			break;
		case Enums::Communication::StartDuel:
			Wire.write("Dra");
			IsInDuel = true;
			Serial.println(F("Starting Duel!"));
			_recievedData = 0;
			break;
		case Enums::Communication::EndDuel:
			Wire.write("gon");
			_recievedData = 0;
			break;
	}
}

void CommunicationsHandler::HandleNewEvent(uint8_t eventType, EventData eventData) {
	String eventString = "";

	switch (eventType) {
		case 1:
			eventString = eventData.monsterZoneEvent;
			break;
		case 2:
			eventString = eventData.spellZoneEvent;
			break;
		default:
			Serial.println(F("[ERROR] Invalid Event Type!"));
			return;
	}
	
	char buffer[13];
	eventString.toCharArray(buffer, 13);
	_newEventData = buffer;

#ifdef DEBUG_CH
	Serial.print("Handling Data: ");
	for (byte i = 0; i < 13; i++) {
		Serial.print(_newEventData[i]);
	}
	Serial.println();
#endif // DEBUG_CH

	digitalWrite(_espInterrupt, LOW);
	delay(10);
	digitalWrite(_espInterrupt, HIGH);

	SendEventSuccess = false;
}

void CommunicationsHandler::GetNextCard() {
	_recievedData = 7;
	EnableWriteMode = false;
}