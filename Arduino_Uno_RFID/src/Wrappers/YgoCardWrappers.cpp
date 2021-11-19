#include "YgoCardWrappers.h"
#include "Arduino.h"

YgoCardWrappers::YgoCardWrappers() 
{
}

bool YgoCardWrappers::isCardMonster(byte cardType)
{
	if (cardType != 50 && cardType != 51) {
		return true;
	}
	return false;
}