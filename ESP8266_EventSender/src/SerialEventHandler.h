#ifndef SerialEventHandler_h
#define SerialEventHandler_h
#include "arduino.h"
#include "Wire.h"

class SerialEventHandler {
private:
	const byte _arduinoAddress = 11;
	const byte _connectionResponseLength = 3;

	byte _message[12] = {
		0x53,
		0x75,
		0x62,
		0x74,
		0x6C,
		0x65,
		0x44,
		0x72,
		0x61,
		0x67,
		0x6F,
		0x6E
	};
	char _response[3] = {
		0x30,
		0x35,
		0x37
	};
	
	bool _debug;	

public:
	SerialEventHandler(bool debug);

	void Initialize();
	void InitializeArduinoCommunication();
	void ResetArduinoConnection();
	bool CheckForResponse();
};

#endif

