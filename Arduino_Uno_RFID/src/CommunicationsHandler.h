#ifndef CommunicationsHandler_h
#define CommunicationsHandler_h
#include "Arduino.h"
#include "Wire.h"
#include "PN532.h"
#include "Core\Entities\Enums.h"

class CommunicationsHandler {
private:
	byte _recievedData;
	bool _hasNewEvent = false;
	char _newEventData[12];
	uint8_t multiplexerAddress = 0x70;

	bool _debug;

public:
	CommunicationsHandler(bool debug = true);

	void InitializeNFCReaders(byte numZones, PN532& reader);
	void HandleRecieve();
	void HandleNewEvent(String eventData);
	void HandleRequest();
	void SelectMultiplexerAddress(uint8_t address);
};

#endif