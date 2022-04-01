#include "Zones.h"

Zone::Zone() 
{
}
Zone::Zone(String name) {
	Name = name;
}

void Zone::UpdateCurrentCard(int cardID, int copyNum, int position) {
	_cardID = cardID;
	_copyNumber = copyNum;
	_position = position;
}

SingleCardZone::SingleCardZone(String name) {
	Name = name;
}

MultiCardZone::MultiCardZone(String name) {
	Name = name;
}