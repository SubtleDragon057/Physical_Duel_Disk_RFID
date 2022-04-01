#ifndef PeripheralsHandler_h
#define PeripheralsHandler_h
#include "arduino.h"
#include "Core\WifiManager.h"
#include "Features\DisplayManager.h"
#include "Secrets.h"
#include "Core\Entities\Enums.h"

class PeripheralsHandler {
private:
	WifiManager _wifiManager;
	DisplayManager _displayManager;
	SECRETS _secrets;

	const byte _arduinoAddress = 11;
	const byte _connectionResponseLength = 3;
	const byte _newDuelData = 12;

	bool SendCommand(Enums::Communication command, String successCode);

public:
	typedef enum {
		UI_Init,
		UI_DeckSelect,
		UI_WriteMode,
		UI_Lobby,
		UI_DuelRoom,
		UI_SpeedDuel,
		UI_ConnectionError
	} UI_Type;

	volatile bool IsBladeConnected = false;
	
	PeripheralsHandler();

	void InitializeCommunications();
	void StartDuelDisk();
	void EndDuel();
	String GetNewEventData();
	void EnableWriteMode();
	void TransmitCard(String cardNumber);
	void Display(UI_Type type, String incomingMessage[]);
};

#endif

