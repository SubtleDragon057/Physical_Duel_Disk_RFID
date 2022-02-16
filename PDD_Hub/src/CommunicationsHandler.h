#ifndef CommunicationsHandler_h
#define CommunicationsHandler_h
#include "arduino.h"
#include "Core\WifiManager.h"
#include "Features\DisplayManager.h"
#include "Secrets.h"
#include "Core\Entities\Enums.h"

class CommunicationsHandler {
private:
	WifiManager _wifiManager;
	DisplayManager _displayManager;
	SECRETS _secrets;

	const byte _arduinoAddress = 11;
	const byte _connectionResponseLength = 3;
	const byte _newDuelData = 12;

	bool CheckForArduino(Enums::Communication command, String successCode);

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

	void Initialize();
	void StartDuelDisk();
	void EndDuel();
	String GetNewEventData();
	String RetryLastCommunication();
	void EnableWriteMode();
	void TransmitCard(String cardNumber);
	void Display(UI_Type type, String incomingMessage[]);
};

#endif

