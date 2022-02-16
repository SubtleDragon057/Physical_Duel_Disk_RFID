#include "DisplayManager.h"
#include "Entities\CrownCorp.h"

DisplayManager::DisplayManager()
{
}

void DisplayManager::Initialize() {
	_display.init();
	_display.flipScreenVertically();
}

void DisplayManager::HandleBasicUI(String incomingMessage[]) {
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

void DisplayManager::HandleLobbyUI(String incomingMessage[]) {
	_display.clear();

	_display.setFont(ArialMT_Plain_10);
	_display.drawString(5, 0, "Lobby");
	_display.drawHorizontalLine(0, 13, 128);
	_display.drawHorizontalLine(0, 14, 128);

	_display.setFont(ArialMT_Plain_16);
	_display.drawString(5, 30, "Room: " + incomingMessage[0]);

	_display.display();
}

void DisplayManager::HandleSpeedDuelUI(String incomingMessage[]) {
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

void DisplayManager::HandleDeckSelectorUI(String incomingMessage[]) {
	_display.clear();

	_display.setFont(ArialMT_Plain_10);
	_display.drawString(5, 0, "Deck Selector");
	_display.drawHorizontalLine(0, 13, 128);
	_display.drawHorizontalLine(0, 14, 128);

	for (byte i = 0; i < sizeof(incomingMessage); i++) {
		_display.drawString(5, i + (15 + 10 * i), incomingMessage[i]);
	}

	_display.display();
}