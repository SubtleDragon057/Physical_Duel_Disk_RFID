#include "Playerstate.h"

//#define DEBUG_PS

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
		if (_singleZones[i].ZoneName != zoneName) continue;
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
		if (_singleZones[i].ZoneName != zoneName) continue;
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
		if (_singleZones[i].ZoneName != zoneName) continue;
		currentCopyNumber = _singleZones[i].CopyNumber();
	}

	return currentCopyNumber;
}

String PlayerState::GetCardPosition(bool isMonsterZone, int zoneNumber) {
	if (isMonsterZone) {
		return _singleZones[zoneNumber].Position();
	}

	return _singleZones[zoneNumber + 3].Position();
}

void PlayerState::UpdateDuelistID(String duelistID, bool isOpponent)
{
	_duelistID = duelistID;
	_isOppnent = isOpponent;
}

void PlayerState::UpdatePlayerstate(int cardID, int copyNumber, String zoneName, String position) {
	
	for (byte i = 0; i < 8; i++) {
		if (_singleZones[i].ZoneName != zoneName) continue;
		_singleZones[i].UpdateCurrentCard(cardID, copyNumber, position);
#ifdef DEBUG_PS
		Serial.printf("Duelist: %s\n", _duelistID.c_str());
		Serial.printf("SingleCard: %i\n", _singleZones[i].CurrentCard());
		Serial.printf("Copy Number: %i\n", _singleZones[i].CopyNumber());
		Serial.printf("Zone: %s\n", _singleZones[i].Name.c_str());
		Serial.printf("Position: %i\n", _singleZones[i].Position());
#endif // DEBUG_PS

		return;
	}

	for (byte i = 0; i < 5; i++) {
		if (_multiZones[i].ZoneName != zoneName) continue;
		
		RemoveFromPreviousZone(cardID, copyNumber);
		
#ifdef DEBUG_PS
		Serial.printf("Duelist: %s\n", _duelistID.c_str());
		Serial.printf("MultiCard: %s\n", _multiZones[i].Name.c_str());
#endif // DEBUG_PS

		return;
	}
}

void PlayerState::UpdateLifepoints(int lifepoints) {
	_lifepoints = lifepoints;

#ifdef DEBUG_PS
	Serial.printf("Duelist: %s, LP: %i\n", _duelistID.c_str(), _lifepoints);
#endif // DEBUG_PS

}

void PlayerState::Clear() {
	UpdateDuelistID("", false);
	
	for (byte i = 0; i < 8; i++) {
		_singleZones[i].UpdateCurrentCard(0, 0, "FaceUp");
	}

	for (byte i = 0; i < 5; i++) {
		
	}
}

void PlayerState::RemoveFromPreviousZone(int cardID, int copyNumber) {
	for (byte i = 0; i < 8; i++) {
		if (_singleZones[i].CurrentCard() != cardID &&
			_singleZones[i].CopyNumber() != copyNumber) continue;
		
		_singleZones[i].UpdateCurrentCard(0, 0, "FaceUp");
	}
}