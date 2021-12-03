#ifndef CommunicationsHandler_h
#define CommunicationsHandler_h
#include "arduino.h"
#include "Wire.h"
#include "Core\WifiManager.h"

class CommunicationsHandler {
private:
	WifiManager _wifiManager;
	const byte _arduinoAddress = 11;
	const byte _connectionResponseLength = 3;
	const byte _newDuelData = 11;
	
	bool _debug;

	bool CheckForArduino();

public:
	CommunicationsHandler(bool debug);

	void Initialize(const char * networkName, const char * networkPass);
	String PollForNewEvent();
};

#endif

