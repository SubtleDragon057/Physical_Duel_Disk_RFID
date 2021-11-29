#include "CommunicationsHandler.h"
#include "Arduino.h"
#include "Wire.h"
#include "Core\Entities\Enums.h"

CommunicationsHandler::CommunicationsHandler(bool debug) {
	_debug = debug;
}

void CommunicationsHandler::Initialize() {
	Serial.println();
	for (uint8_t t = 3; t > 0; t--) {
		Serial.printf("[SETUP] BOOT WAIT %d...\n", t);
		Serial.flush();
		delay(1000);
	}

	bool isArduinoConnected = false;
	while (!isArduinoConnected) {
		isArduinoConnected = CheckForArduino();
		delay(50);
	}

	delay(1000);
	Serial.println("[SETUP] Arduino Connected");
	Serial.println();
}

String CommunicationsHandler::PollForNewEvent() {

	Wire.beginTransmission(_arduinoAddress);
	Wire.write(Enums::Communication::HasNewEvent);
	Wire.endTransmission();
	
	const byte constByte = 1;
	Wire.requestFrom(_arduinoAddress, constByte);

	bool hasEvent = false;
	while (Wire.available()) {
		hasEvent = Wire.read();
	}

	if (hasEvent) {		
		Wire.beginTransmission(_arduinoAddress);
		Wire.write(Enums::Communication::GetEventInfo);
		Wire.endTransmission();

		Wire.requestFrom(_arduinoAddress, _newDuelData);

		String eventData = "";

		while (Wire.available()) {			
			char message = Wire.read();
			eventData += message;
		}

		return eventData;
	}
	
	return "No Events!!";
}

bool CommunicationsHandler::CheckForArduino() {
	bool arduinoConnected = false;
	String response = "";

	Wire.beginTransmission(_arduinoAddress);
	Wire.write(Enums::Communication::Connection);
	Wire.endTransmission();

	Wire.requestFrom(_arduinoAddress, _connectionResponseLength);

	while (Wire.available()) {
		char message = Wire.read();
		response += message;
	}

	return response == "057";
}