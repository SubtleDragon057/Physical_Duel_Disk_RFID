#include "YgoCard.h"

YgoCard::YgoCard(String serialNumber, Enums::CardPosition position)
{
	_serialNumber = serialNumber;
	_position = position;
}

void YgoCard::UpdateCard(String serialNumber, Enums::CardPosition newPosition)
{
	_serialNumber = serialNumber;
	_position = newPosition;
}