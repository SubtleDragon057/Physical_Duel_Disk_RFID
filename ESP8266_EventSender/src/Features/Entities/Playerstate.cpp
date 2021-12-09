#include "Playerstate.h"

PlayerState::PlayerState() 
{
}

bool PlayerState::IsZoneEmpty(bool isMonsterZone, int zoneNumber) {
	if (isMonsterZone) {
		return _singleZones[zoneNumber].CurrentCard() == 0;
	}

	return _singleZones[zoneNumber + 3].CurrentCard() == 0;
}
bool PlayerState::IsZoneEmpty(String zoneName) {
	int currentCardID = 0;
	for (byte i = 0; i < 8; i++) {
		if (_singleZones[i].Name != zoneName) continue;
		currentCardID = _singleZones[i].CurrentCard();
	}

	return currentCardID == 0;
}

int PlayerState::GetCardID(bool isMonsterZone, int zoneNumber) {
	if (isMonsterZone) {
		return _singleZones[zoneNumber].CurrentCard();
	}

	return _singleZones[zoneNumber + 3].CurrentCard();
}
int PlayerState::GetCardID(String zoneName) {
	int currentCardID = 0;
	for (byte i = 0; i < 8; i++) {
		if (_singleZones[i].Name != zoneName) continue;
		currentCardID = _singleZones[i].CurrentCard();
	}

	return currentCardID;
}

int PlayerState::GetCopyNumber(bool isMonsterZone, int zoneNumber) {
	if (isMonsterZone) {
		return _singleZones[zoneNumber].CopyNumber();
	}

	return _singleZones[zoneNumber + 3].CopyNumber();
}
int PlayerState::GetCopyNumber(String zoneName) {
	int currentCopyNumber = 0;
	for (byte i = 0; i < 8; i++) {
		if (_singleZones[i].Name != zoneName) continue;
		currentCopyNumber = _singleZones[i].CopyNumber();
	}

	return currentCopyNumber;
}

void PlayerState::UpdateDuelistID(String duelistID, bool isOpponent)
{
	_duelistID = duelistID;
	_isOppnent = isOpponent;
}

void PlayerState::UpdatePlayerstate(int cardID, int copyNumber, String zoneName) {
	
	for (byte i = 0; i < 8; i++) {
		if (_singleZones[i].Name != zoneName) continue;
		_singleZones[i].UpdateCurrentCard(cardID, copyNumber);
		/*Serial.printf("Duelist: %s\n", _duelistID.c_str());
		Serial.printf("SingleCard: %i\n", _singleZones[i].CurrentCard());
		Serial.printf("Copy Number: %i\n", _singleZones[i].CopyNumber());
		Serial.printf("Zone: %s\n", _singleZones[i].Name.c_str());*/
		return;
	}

	for (byte i = 0; i < 5; i++) {
		if (_multiZones[i].Name != zoneName) continue;
		
		RemoveFromPreviousZone(cardID, copyNumber);

		/*Serial.printf("Duelist: %s\n", _duelistID.c_str());
		Serial.printf("MultiCard: %s\n", _multiZones[i].Name.c_str());*/
		return;
	}
}

void PlayerState::Clear() {
	UpdateDuelistID("", false);
	
	for (byte i = 0; i < 8; i++) {
		_singleZones[i].UpdateCurrentCard(0, 0);
	}

	for (byte i = 0; i < 5; i++) {
		
	}
}

void PlayerState::RemoveFromPreviousZone(int cardID, int copyNumber) {
	for (byte i = 0; i < 8; i++) {
		if (_singleZones[i].CurrentCard() != cardID &&
			_singleZones[i].CopyNumber() != copyNumber) continue;
		
		_singleZones[i].UpdateCurrentCard(0, 0);
	}
}