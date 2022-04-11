#include "PeripheralsHandler.h"
#include "Wire.h"

//#define DEBUG_CH

PeripheralsHandler::PeripheralsHandler() {
	for (byte i = 0; i < 4; i++) {
		pinMode(_muxPins[i], OUTPUT);
	}
}

void PeripheralsHandler::InitializeCommunications() {
	_displayManager.Initialize();
	
	String text[1] = {};
	for (uint8_t t = 3; t > 0; t--) {
		text[0] = "[SETUP] BOOT WAIT " + String(t);
		Display(UI_Init, text);
		delay(1000);
	}

	text[0] = "[SETUP] Initialize Zones";
	Display(UI_Init, text);
	
	// TODO: PN532 setup status

	delay(1000);

	text[0] = "[SETUP] Connecting Wifi";
	Display(UI_Init, text);
	_wifiManager.Connect(_secrets.networkName, _secrets.networkPass);

	text[0] = "[SETUP] Configuring Decks";
	Display(UI_Init, text);
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

void PeripheralsHandler::SelectMultiplexerAddress(uint8_t address) {
	for (byte i = 0; i < 4; i++) {
		digitalWrite(_muxPins[i], _muxChannels[address][i]);
	}
}