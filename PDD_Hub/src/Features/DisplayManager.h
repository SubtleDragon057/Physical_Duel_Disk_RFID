#ifndef DisplayManager_h
#define DisplayManager_h
#include "arduino.h"
#include "Wire.h"
#include "SSD1306Wire.h"

class DisplayManager {
private:
	SSD1306Wire _display = SSD1306Wire(0x3c, SDA, SCL);

public:
	DisplayManager();
	
	void Initialize();
	void HandleBasicUI(String incomingMessage[]);
	void HandleLobbyUI(String incomingMessage[]);
	void HandleSpeedDuelUI(String incomingMessage[]);
	void HandleDeckSelectorUI(String incomingMessage[]);
};

#endif