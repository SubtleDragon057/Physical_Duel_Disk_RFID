#ifndef CommunicationsHandler_h
#define CommunicationsHandler_h
#include "arduino.h"
#include "Core\WifiManager.h"
#include "SSD1306Wire.h"
#include "Core\Entities\Enums.h"

class CommunicationsHandler {
private:
	WifiManager _wifiManager;
	SSD1306Wire _display = SSD1306Wire(0x3c, SDA, SCL);

	const byte _arduinoAddress = 11;
	const byte _connectionResponseLength = 3;
	const byte _newDuelData = 12;

	bool CheckForArduino(Enums::Communication command, String successCode);
	void HandleBasicUI(String incomingMessage[]);
	void HandleLobbyUI(String incomingMessage[]);
	void HandleSpeedDuelUI(String incomingMessage[]);
	void HandleDeckSelectorUI(String incomingMessage[]);

public:
	typedef enum {
		UI_Init,
		UI_DeckSelect,
		UI_WriteMode,
		UI_Lobby,
		UI_DuelRoom,
		UI_SpeedDuel
	} UI_Type;
	
	CommunicationsHandler();

	void Initialize(const char * networkName, const char * networkPass);
	void StartDuelDisk();
	void EndDuel();
	String GetNewEventData();
	void EnableWriteMode();
	void TransmitCard(String cardNumber);
	void Display(UI_Type type, String incomingMessage[]);
};

#endif

