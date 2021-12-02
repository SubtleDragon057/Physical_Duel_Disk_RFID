#include "Playerstate.h"
#include "Arduino.h"

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
	for (int i = 0; i < _numSingleZones; i++) {
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
	for (int i = 0; i < _numSingleZones; i++) {
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
	for (int i = 0; i < _numSingleZones; i++) {
		if (_singleZones[i].Name != zoneName) continue;
		currentCopyNumber = _singleZones[i].CopyNumber();
	}

	return currentCopyNumber;
}

void PlayerState::UpdateDuelistID(String duelistID, bool isOpponent)
{
	_duelistID = duelistID;
	_isOppnent = isOpponent;

	Serial.print("Duelist: ");
	Serial.println(_duelistID);
	Serial.print("Is Opponent: ");
	Serial.println(_isOppnent);
	delay(50);
}

void PlayerState::UpdatePlayerstate(int cardID, int copyNumber, String zoneName) {
	for (int i = 0; i < _numSingleZones; i++) {
		if (_singleZones[i].Name != zoneName) continue;
		_singleZones[i].UpdateCurrentCard(cardID, copyNumber);
		Serial.print("Duelist: ");
		Serial.println(_duelistID);
		Serial.print("SingleCard: ");
		Serial.println(_singleZones[i].CurrentCard());
		Serial.print("Copy Number: ");
		Serial.println(_singleZones[i].CopyNumber());
		Serial.print("Zone: ");
		Serial.println(_singleZones[i].Name);
		return;
	}

	for (int i = 0; i < _numMultiZones; i++) {
		if (_multiZones[i].Name != zoneName) continue;
		
		RemoveFromPreviousZone(cardID, copyNumber);

		Serial.print("Duelist: ");
		Serial.println(_duelistID);
		Serial.print("MultiCard: ");
		Serial.println(_multiZones[i].Name);
		return;
	}
}

void PlayerState::Clear() {
	UpdateDuelistID("", false);
	
	for (int i = 0; i < _numSingleZones; i++) {
		_singleZones[i].UpdateCurrentCard(0, 0);
	}

	for (int i = 0; i < _numMultiZones; i++) {
		
	}
}

void PlayerState::RemoveFromPreviousZone(int cardID, int copyNumber) {
	for (int i = 0; i < _numSingleZones; i++) {
		if (_singleZones[i].CurrentCard() != cardID &&
			_singleZones[i].CopyNumber() != copyNumber) continue;
		
		_singleZones[i].UpdateCurrentCard(0, 0);
	}
}