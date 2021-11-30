#include "Playerstate.h"
#include "Arduino.h"

Playerstate::Playerstate() 
{
}

bool Playerstate::IsZoneEmpty(bool isMonsterZone, int zoneNumber) {
	if (isMonsterZone) {
		return _singleZones[zoneNumber].CurrentCard() == 0;
	}

	return _singleZones[zoneNumber + 3].CurrentCard() == 0;
}

int Playerstate::GetCardID(bool isMonsterZone, int zoneNumber) {
	if (isMonsterZone) {
		return _singleZones[zoneNumber].CurrentCard();
	}

	return _singleZones[zoneNumber + 3].CurrentCard();
}

int Playerstate::GetCopyNumber(bool isMonsterZone, int zoneNumber) {
	if (isMonsterZone) {
		return _singleZones[zoneNumber].CopyNumber();
	}

	return _singleZones[zoneNumber + 3].CopyNumber();
}

void Playerstate::UpdateDuelistID(String duelistID, bool isOpponent)
{
	_duelistID = duelistID;
	_isOppnent = isOpponent;

	Serial.print("Duelist: ");
	Serial.println(_duelistID);
	Serial.print("Is Opponent: ");
	Serial.println(_isOppnent);
	delay(50);
}

void Playerstate::UpdatePlayerstate(int cardID, int copyNumber, String zoneName) {
	for (int i = 0; i < 8; i++) {
		if (_singleZones[i].Name != zoneName) continue;
		_singleZones[i].UpdateCurrentCard(cardID, copyNumber);
		Serial.print("Duelist: ");
		Serial.println(_duelistID);
		Serial.print("SingleCard: ");
		Serial.println(_singleZones[i].CurrentCard());
		Serial.print("Copy Number: ");
		Serial.println(_singleZones[i].CopyNumber());
		return;
	}

	for (int i = 0; i < 5; i++) {
		if (_multiZones[i].Name != zoneName) continue;
		Serial.print("Duelist: ");
		Serial.println(_duelistID);
		Serial.print("MultiCard: ");
		Serial.println(_multiZones[i].Name);
		return;
	}
}