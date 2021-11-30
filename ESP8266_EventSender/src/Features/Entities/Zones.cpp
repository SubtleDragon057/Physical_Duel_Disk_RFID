#include "Zones.h"

Zone::Zone() 
{
}
Zone::Zone(String name) {
	Name = name;
}

void Zone::UpdateCurrentCard(int cardID, int copyNum) {
	_cardID = cardID;
	_copyNumber = copyNum;
}

SingleCardZone::SingleCardZone(String name) {
	Name = name;
}

MultiCardZone::MultiCardZone(String name) {
	Name = name;
}