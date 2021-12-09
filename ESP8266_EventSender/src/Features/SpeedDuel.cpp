#include "SpeedDuel.h"

SpeedDuel::SpeedDuel()
{
}

// TODO: Check for Face-Down Spell and activate it before declare event
EventData SpeedDuel::HandleActivateSpell(String socketID, int zoneNumber) {
	int spellID = _duelState.GetCardID(socketID, zoneNumber, false);
	int copyNum = _duelState.GetCopyNumber(socketID, zoneNumber, false);
	if (spellID == 0) {
		Serial.printf("No valid Spell on Zone: %i\n", zoneNumber);
		return EventData();
	}

	Serial.printf("Spell %i activated on zone: %i\n", spellID, zoneNumber);
	return EventData("card:declare", spellID, copyNum);
}

EventData SpeedDuel::HandleActivateMonsterEffect(String socketID, int zoneNumber) {
	int monsterID = _duelState.GetCardID(socketID, zoneNumber, true);
	int copyNum = _duelState.GetCopyNumber(socketID, zoneNumber, true);
	if (monsterID == 0) {
		Serial.printf("No valid Monster on Zone: %i\n", zoneNumber);
		return EventData();
	}

	Serial.printf("Monster %i activated on zone: %i\n", monsterID, zoneNumber);
	return EventData("card:declare", monsterID, copyNum);
}

void SpeedDuel::HandleAttackEvent(String socketID, int zoneNumber) {
	int monsterID = _duelState.GetCardID(socketID, zoneNumber, true);
	if (monsterID == 0) {
		Serial.printf("No valid Monster on Zone: %i\n", zoneNumber);
		return;
	}

	_attackingMonster = zoneNumber;
	HasAttackTarget = false;
	Serial.println("Declare an Attack Target!");
}

EventData SpeedDuel::HandleMonsterAttack(String socketID, String zoneName) {
	if (!CheckForValidTarget(zoneName)) {
		Serial.printf("No Targets on Zone: %i\n", zoneName);
		Serial.printf("Hint: Hold Buttons 1 & 5 for a Direct Attack!\n");
		return EventData();
	}

	HasAttackTarget = true;
	int monsterID = _duelState.GetCardID(socketID, _attackingMonster, true);
	int copyNum = _duelState.GetCopyNumber(socketID, _attackingMonster, true);

	return EventData("card:attack", monsterID, copyNum, zoneName);
}

void SpeedDuel::UpdateDuelistIDs(String socketID, String duelist1, String duelist2)
{
	_duelState.UpdateDuelistIDs(socketID, duelist1, duelist2);
}

void SpeedDuel::UpdateDuelState(String newDuelState)
{
	_duelState.UpdateDuelState(newDuelState);
}
void SpeedDuel::UpdateDuelState(String duelistID, int cardID, int copyNumber, String zoneName)
{
	_duelState.UpdateDuelState(duelistID, cardID, copyNumber, zoneName);
}

void SpeedDuel::ClearDuelStates() {
	_duelState.ClearPlayerStates();
}

void SpeedDuel::UpdatePhase(String newPhase, bool isOpponentsTurn) {
	_duelState.UpdatePhase(newPhase, isOpponentsTurn);
}

String SpeedDuel::GetPhase() {
	return _duelState.GetPhase();
}

bool SpeedDuel::CheckForValidTarget(String targetZoneName) {
	if (targetZoneName == "hand") return true;
	
	int targetID = _duelState.GetCardID("", targetZoneName);
	return targetID != 0;
}

String SpeedDuel::HandleChangePhase() {
	if (_duelState.IsOpponentsTurn) return String();

	if (_duelState.CurrentPhase == "drawPhase") {
		_duelState.UpdatePhase("standbyPhase");
	}
	else if (_duelState.CurrentPhase == "standbyPhase") {
		_duelState.UpdatePhase("mainPhase1");
	}
	else if (_duelState.CurrentPhase == "mainPhase1") {
		_duelState.UpdatePhase("battlePhase");
	}
	else if (_duelState.CurrentPhase == "battlePhase") {
		_duelState.UpdatePhase("endPhase");
	}
	else if (_duelState.CurrentPhase == "endPhase") {
		_duelState.UpdatePhase("passTurn", true);
	}

	return _duelState.CurrentPhase;
}