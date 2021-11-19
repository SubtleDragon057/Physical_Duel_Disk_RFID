#ifndef YgoCard_h
#define YgoCard_h
#include "Arduino.h"
#include "Enums.h"

class YgoCard {
public:

	String GetSerialNumber();
	int GetPosition();
	void UpdateCard(String serialNumber, Enums::CardPosition newPosition);

protected:
	String _serialNumber;
	Enums::CardPosition _position;
};

class Monster : public YgoCard {
public:
	Monster();
	Monster(String serialNum, Enums::CardPosition position);
};

class Spell : public YgoCard {
public:
	Spell();
	Spell(String serialNum, Enums::CardPosition position);
};

#endif