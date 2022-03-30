#include "PeripheralsHandler.h"
#include "Wire.h"

//#define DEBUG_CH

PeripheralsHandler::PeripheralsHandler()
{
}

void PeripheralsHandler::InitializeCommunications() {
	_displayManager.Initialize();
	
	String text[1] = {};
	for (uint8_t t = 3; t > 0; t--) {
		text[0] = "[SETUP] BOOT WAIT " + String(t);
		Display(UI_Init, text);
		delay(1000);
	}

	text[0] = "[SETUP] Sync Blade I2C";
	Display(UI_Init, text);
	
	bool isArduinoConnected = false;
	do {
		isArduinoConnected = SendCommand(Enums::Communication::Connection, "057");
		delay(50);
	} while (!isArduinoConnected);

	text[0] = "[SETUP] Sync Blade Int";
	Display(UI_Init, text);
	do {
		delay(50);
	} while (!IsBladeConnected);

	delay(1000);

	text[0] = "[SETUP] Connecting Wifi";
	Display(UI_Init, text);
	_wifiManager.Connect(_secrets.networkName, _secrets.networkPass);

	text[0] = "[SETUP] Configuring Decks";
	Display(UI_Init, text);
}

void PeripheralsHandler::StartDuelDisk() {
	bool isArduinoConnected = false;
	do {
		isArduinoConnected = SendCommand(Enums::Communication::StartDuel, "Dra");
		delay(50);
	} while (!isArduinoConnected);
}

void PeripheralsHandler::EndDuel() {
	bool isArduinoConnected = false;
	do {		
		isArduinoConnected = SendCommand(Enums::Communication::EndDuel, "gon");
		delay(50);
	} while (!isArduinoConnected);
}

String PeripheralsHandler::GetNewEventData() {
	
	bool success;
	String eventData = "";
	for (byte i = 5; i > 0; i--) {
		Wire.requestFrom(_arduinoAddress, _newDuelData);

		while (Wire.available()) {
			char message = Wire.read();

			// If char is not 0-9 an error occurred
			if (message < 48 || message > 57) {
				success = false;
				break;
			}

			eventData += message;
			success = true;
		}

		if (success) break;
		eventData = "";
		delay(10);
	}

	if (!success) {
		SendCommand(Enums::Communication::CommunicationFailure, "tle");		
		return "Failure";
	}

	SendCommand(Enums::Communication::ClearEventData, "tle");
	return eventData;
}

void PeripheralsHandler::EnableWriteMode() {
	String text[] = { "Configuring Deck" };
	Display(UI_WriteMode, text);

	bool isArduinoConnected = false;
	do {		
		isArduinoConnected = SendCommand(Enums::Communication::EnterWriteMode, "Sub");
		delay(100);
	} while (!isArduinoConnected);
}

void PeripheralsHandler::TransmitCard(String cardNumber) {
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

bool PeripheralsHandler::SendCommand(Enums::Communication command, String successCode) {
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

void PeripheralsHandler::Display(UI_Type type, String incomingMessage[]) {
#ifdef DEBUG_CH
	Serial.printf("Display(UI Type: %i)\n", type);
#endif // DEBUG_CH

	switch (type) {
	case UI_Lobby:
		_displayManager.HandleLobbyUI(incomingMessage);
		break;
	case UI_DeckSelect:
		_displayManager.HandleDeckSelectorUI(incomingMessage);
		break;
	case UI_SpeedDuel:
		_displayManager.HandleSpeedDuelUI(incomingMessage);
		break;
	default:
		_displayManager.HandleBasicUI(incomingMessage);
	}
}