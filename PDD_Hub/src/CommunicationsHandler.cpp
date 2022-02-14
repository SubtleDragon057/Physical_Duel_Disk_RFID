#include "CommunicationsHandler.h"
#include "Wire.h"
#include "CrownCorp.h"

//#define DEBUG_CH

CommunicationsHandler::CommunicationsHandler()
{
}

void CommunicationsHandler::Initialize(const char * networkName, const char * networkPass) {
	_display.init();
	_display.flipScreenVertically();
	
	String text[1] = {};
	for (uint8_t t = 3; t > 0; t--) {
		text[0] = "[SETUP] BOOT WAIT " + String(t);
		Display(UI_Init, text);
		delay(1000);
	}

	text[0] = "[SETUP] Syncing RFIDs";
	Display(UI_Init, text);
	
	// TODO: Add test for interrupt pin
	bool isArduinoConnected = false;
	do {
		isArduinoConnected = CheckForArduino(Enums::Communication::Connection, "057");
		delay(50);
	} while (!isArduinoConnected);

	delay(1000);

	text[0] = "[SETUP] Connecting Wifi";
	Display(UI_Init, text);
	_wifiManager.Connect(networkName, networkPass);

	text[0] = "[SETUP] Configuring Decks";
	Display(UI_Init, text);
}

void CommunicationsHandler::StartDuelDisk() {
	bool isArduinoConnected = false;
	do {
		isArduinoConnected = CheckForArduino(Enums::Communication::StartDuel, "Dra");
		delay(50);
	} while (!isArduinoConnected);
}

void CommunicationsHandler::EndDuel() {
	bool isArduinoConnected = false;
	do {		
		isArduinoConnected = CheckForArduino(Enums::Communication::EndDuel, "gon");
		delay(50);
	} while (!isArduinoConnected);
}

String CommunicationsHandler::GetNewEventData() {
	Wire.requestFrom(_arduinoAddress, _newDuelData);

	String eventData = "";
	while (Wire.available()) {
		char message = Wire.read();
		eventData += message;
	}

	return eventData;
}

void CommunicationsHandler::EnableWriteMode() {
	String text[] = { "Configuring Deck" };
	Display(UI_WriteMode, text);

	bool isArduinoConnected = false;
	do {		
		isArduinoConnected = CheckForArduino(Enums::Communication::EnterWriteMode, "Sub");
		delay(100);
	} while (!isArduinoConnected);
}

void CommunicationsHandler::TransmitCard(String cardNumber) {
	char cardNumAsChar[10];
	cardNumber.toCharArray(cardNumAsChar, 10);
	
	// Send Card
	Wire.beginTransmission(_arduinoAddress);
	Wire.write(cardNumAsChar);
	Wire.endTransmission();

	// TODO: Add check from UI rather than Serial
	// Wait for card to be written - Temp disabled to clear I2C line
	while (1) {}
}

bool CommunicationsHandler::CheckForArduino(Enums::Communication command, String successCode) {
	String response = "";

	Wire.beginTransmission(_arduinoAddress);
	Wire.write(command);
	Wire.endTransmission();

	Wire.requestFrom(_arduinoAddress, _connectionResponseLength);

	while (Wire.available()) {
		char message = Wire.read();
		response += message;
	}

	return response == successCode;
}

void CommunicationsHandler::Display(UI_Type type, String incomingMessage[]) {
#ifdef DEBUG_CH
	Serial.printf("Display(UI Type: %i)\n", type);
#endif // DEBUG_CH


	switch (type) {
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
	_display.drawString(5, 0, incomingMessage[2]);
	_display.drawHorizontalLine(0, 13, 128);
	_display.drawHorizontalLine(0, 14, 128);

	_display.drawString(5, 52, incomingMessage[1]);
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