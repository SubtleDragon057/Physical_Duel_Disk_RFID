#include "SerialEventHandler.h"
#include "Arduino.h"
#include "Wire.h"

SerialEventHandler::SerialEventHandler(bool debug)
{
	_debug = debug;
}

void SerialEventHandler::Initialize()
{
	Serial.println();
	for (uint8_t t = 3; t > 0; t--) {
		Serial.printf("[SETUP] BOOT WAIT %d...\n", t);
		Serial.flush();
		delay(1000);
	}

	InitializeArduinoCommunication();
}

void SerialEventHandler::InitializeArduinoCommunication() {

	bool pingArduino = false;
	while (!pingArduino) {

		Serial.println("Connecting");
		delay(500);

		pingArduino = CheckForResponse();
	}

	Serial.println("Arduino Is Connected");
	Serial.println();
}

bool SerialEventHandler::CheckForResponse() {
	bool arduinoConnected = false;
	String response = "";

	Wire.beginTransmission(_arduinoAddress);
	Wire.write("SubtleDragon");
	Wire.endTransmission();

	Wire.requestFrom(_arduinoAddress, _connectionResponseLength);

	while (Wire.available()) {
		char message = Wire.read();
		response += message;
	}
	
	Serial.println(response);
	return response == "057";
}