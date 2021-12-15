#ifndef YgoCard_h
#define YgoCard_h
#include "Arduino.h"
#include "Enums.h"

class YgoCard {
private:
	String _serialNumber;
	Enums::CardPosition _position;

public:
	YgoCard(String serialNum, Enums::CardPosition position);

	String GetSerialNumber() { return _serialNumber; }
	int GetPosition() { return _position; }
	void UpdateCard(String serialNumber, Enums::CardPosition newPosition);
};

#endif