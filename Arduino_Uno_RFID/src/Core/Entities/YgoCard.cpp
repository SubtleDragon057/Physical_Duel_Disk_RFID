#include "Arduino.h"
#include "YgoCard.h"
#include "Enums.h"

Monster::Monster() 
{
}
Monster::Monster(String serialNumber, Enums::CardPosition position)
{
	_serialNumber = serialNumber;
	_position = position;
}

void Monster::UpdateCard(String serialNumber, Enums::CardPosition newPosition)
{
	_serialNumber = serialNumber;
	_position = newPosition;
}

Spell::Spell() 
{
}
Spell::Spell(String serialNumber, Enums::CardPosition position)
{
	_serialNumber = serialNumber;
	_position = position;
}

void Spell::UpdateCard(String serialNumber, Enums::CardPosition newPosition)
{
	_serialNumber = serialNumber;
	_position = newPosition;
}
