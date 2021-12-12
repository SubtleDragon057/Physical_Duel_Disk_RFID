#ifndef YgoCard_h
#define YgoCard_h
#include "Arduino.h"
#include "Enums.h"

class Monster {
private:
	String _serialNumber;
	Enums::CardPosition _position;

public:
	Monster(String serialNum, Enums::CardPosition position);

	String GetSerialNumber() { return _serialNumber; }
	int GetPosition() { return _position; }
	void UpdateCard(String serialNumber, Enums::CardPosition newPosition);
};

class Spell {
private:
	String _serialNumber;
	Enums::CardPosition _position;

public:
	Spell(String serialNum, Enums::CardPosition position);

	String GetSerialNumber() { return _serialNumber; }
	int GetPosition() { return _position; }
	void UpdateCard(String serialNumber, Enums::CardPosition newPosition);
};

#endif