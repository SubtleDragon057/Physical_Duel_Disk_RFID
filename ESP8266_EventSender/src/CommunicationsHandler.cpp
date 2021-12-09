#include "CommunicationsHandler.h"
#include "Wire.h"
#include "Core\Entities\Enums.h"
#include "CrownCorp.h"

CommunicationsHandler::CommunicationsHandler()
{
}

void CommunicationsHandler::Initialize(const char * networkName, const char * networkPass) {
	_display.init();
	_display.flipScreenVertically();
	String text[1] = { "Display: Init" };
	Display(UI_Init, text);
	
	for (uint8_t t = 3; t > 0; t--) {
		text[0] = "[SETUP] BOOT WAIT " + String(t);
		Display(UI_Init, text);
		delay(1000);
	}

	bool isArduinoConnected = false;
	do {
		isArduinoConnected = CheckForArduino();
		delay(50);
	} while (!isArduinoConnected);

	text[0] = "[SETUP] Arduino Connected";
	Display(UI_Init, text);
	delay(1000);

	text[0] = "[SETUP] Connecting Wifi";
	Display(UI_Init, text);
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

void CommunicationsHandler::Display(UI_Type type, String incomingMessage[])
{
	switch (type)
	{
		case UI_Lobby:
			HandleLobbyUI(incomingMessage);
			break;
		case UI_DeckSelect:
			HandleDeckSelectorUI(incomingMessage);
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

void CommunicationsHandler::HandleBasicUI(String incomingMessage[]) {
	_display.clear();

	_display.setFont(ArialMT_Plain_10);
	_display.drawString(5, 0, "Initialize");
	_display.drawHorizontalLine(0, 13, 128);
	_display.drawHorizontalLine(0, 14, 128);
	
	_display.drawXbm(32, 20, CC_Logo_Width, CC_Logo_Height, CC_Logo_Bytes);

	_display.setTextAlignment(TEXT_ALIGN_CENTER);
	_display.drawString(64, 51, incomingMessage[0]);
	_display.setTextAlignment(TEXT_ALIGN_LEFT);

	_display.display();
}

void CommunicationsHandler::HandleLobbyUI(String incomingMessage[]) {
	_display.clear();

	_display.setFont(ArialMT_Plain_10);
	_display.drawString(5, 0, "Lobby");
	_display.drawHorizontalLine(0, 13, 128);
	_display.drawHorizontalLine(0, 14, 128);

	_display.setFont(ArialMT_Plain_16);
	_display.drawString(5, 30, "Room: " + incomingMessage[0]);

	_display.display();
}

void CommunicationsHandler::HandleSpeedDuelUI(String incomingMessage[]) {
	_display.clear();

	_display.setFont(ArialMT_Plain_10);
	_display.drawString(5, 0, "Opp: 4000");
	_display.drawHorizontalLine(0, 13, 128);
	_display.drawHorizontalLine(0, 14, 128);

	_display.drawString(5, 52, "Player: 4000");
	_display.drawHorizontalLine(0, 49, 128);
	_display.drawHorizontalLine(0, 50, 128);

	_display.setFont(ArialMT_Plain_16);
	_display.drawString(5, 23, incomingMessage[0]);

	_display.display();
}

void CommunicationsHandler::HandleDeckSelectorUI(String incomingMessage[]) {
	_display.clear();

	_display.setFont(ArialMT_Plain_10);
	_display.drawString(5, 0, "Deck Selector");
	_display.drawHorizontalLine(0, 13, 128);
	_display.drawHorizontalLine(0, 14, 128);

	for (byte i = 0; i < sizeof(incomingMessage); i++) {
		_display.drawString(5, i + (15 + 10*i), incomingMessage[i]);
	}

	_display.display();
}