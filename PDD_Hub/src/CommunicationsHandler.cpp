#include "CommunicationsHandler.h"
#include "Wire.h"

//#define DEBUG_CH

CommunicationsHandler::CommunicationsHandler()
{
}

void CommunicationsHandler::Initialize() {
	_displayManager.Initialize();
	
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
	_wifiManager.Connect(_secrets.networkName, _secrets.networkPass);

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
	bool success;
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

	if (!success) {
		eventData = RetryLastCommunication();
	}
	else {
		Wire.beginTransmission(_arduinoAddress);
		Wire.write(Enums::Communication::ClearEventData);
		Wire.endTransmission();
	}

	return eventData;
}

String CommunicationsHandler::RetryLastCommunication() {
	
	bool success;
	String eventData = "";
	for (byte i = 5; i > 0; i--) {
		eventData = "";
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
	}

	if (success) {
		Wire.beginTransmission(_arduinoAddress);
		Wire.write(Enums::Communication::ClearEventData);
		Wire.endTransmission();

		return eventData;
	}

	Wire.beginTransmission(_arduinoAddress);
	Wire.write(Enums::Communication::CommunicationFailure);
	Wire.endTransmission();

	return "Failure";
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