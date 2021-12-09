#include "CommunicationsHandler.h"
#include "Wire.h"
#include "Core\Entities\Enums.h"
#include "CrownCorp.h"

CommunicationsHandler::CommunicationsHandler()
{
}

void CommunicationsHandler::Initialize(const char * networkName, const char * networkPass) {
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
	Serial.printf("[SETUP] Arduino Connected\n");

	_display.init();
	_display.flipScreenVertically();
	Display(UI_Init, "Display: Init");

	_wifiManager.Connect(networkName, networkPass);
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

void CommunicationsHandler::Display(UI_Type type, String incomingMessage)
{
	switch (type)
	{
		case UI_Lobby:
			HandleLobbyUI(incomingMessage);
			break;
		case UI_SpeedDuel:
			HandleSpeedDuelUI(incomingMessage);
			break;
		default:
			HandleBasicUI(incomingMessage);
	}
}

bool CommunicationsHandler::CheckForArduino() {
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

void CommunicationsHandler::HandleBasicUI(String incomingMessage) {
	_display.clear();

	_display.setFont(ArialMT_Plain_10);
	_display.drawString(5, 0, "Initialize");
	_display.drawHorizontalLine(0, 13, 128);
	_display.drawHorizontalLine(0, 14, 128);
	
	_display.drawFastImage(32, 19, CC_Image_Width, CC_Image_Height, CC_Image_Bits);

	_display.display();
}

void CommunicationsHandler::HandleLobbyUI(String incomingMessage) {
	_display.clear();

	_display.setFont(ArialMT_Plain_10);
	_display.drawString(5, 0, "Lobby");
	_display.drawHorizontalLine(0, 13, 128);
	_display.drawHorizontalLine(0, 14, 128);

	_display.setFont(ArialMT_Plain_16);
	_display.drawString(5, 30, "Room: " + incomingMessage);

	_display.display();
}

void CommunicationsHandler::HandleSpeedDuelUI(String incomingMessage) {
	_display.clear();

	_display.setFont(ArialMT_Plain_10);
	_display.drawString(5, 0, "Opp: 4000");
	_display.drawHorizontalLine(0, 13, 128);
	_display.drawHorizontalLine(0, 14, 128);

	_display.drawString(5, 52, "Player: 4000");
	_display.drawHorizontalLine(0, 49, 128);
	_display.drawHorizontalLine(0, 50, 128);

	_display.setFont(ArialMT_Plain_16);
	_display.drawString(5, 23, incomingMessage);

	_display.display();
}