#include "Arduino.h"
#include "YgoCard.h"
#include "Enums.h"

String YgoCard::GetSerialNumber()
{
	return _serialNumber;
}

int YgoCard::GetPosition()
{
	return _position;
}

void YgoCard::UpdateCard(String serialNumber, Enums::CardPosition newPosition)
{	
	_serialNumber = serialNumber;
	_position = newPosition;
}

Monster::Monster() 
{
}
Monster::Monster(String serialNumber, Enums::CardPosition position)
{
	_serialNumber = serialNumber;
	_position = position;
}

Spell::Spell() 
{
}
Spell::Spell(String serialNumber, Enums::CardPosition position)
{
	_serialNumber = serialNumber;
	_position = position;
}