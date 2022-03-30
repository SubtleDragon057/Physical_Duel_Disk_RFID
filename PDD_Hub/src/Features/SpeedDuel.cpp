#include "SpeedDuel.h"

SpeedDuel::SpeedDuel()
{
}

EventData SpeedDuel::HandleActivateSpell(String socketID, int zoneNumber) {
	int spellID = _duelState.GetCardID(socketID, zoneNumber, false);
	int copyNum = _duelState.GetCopyNumber(socketID, zoneNumber, false);
	int position = _duelState.GetCardPosition(socketID, zoneNumber, false);
	if (spellID == 0) {
		Serial.printf("No valid Spell on Zone: %i\n", zoneNumber);
		return EventData();
	}

	if (position == Position::FaceDown) {
		String zone = GetZoneName(zoneNumber + 3);
		_duelState.UpdateDuelState(socketID, spellID, copyNum, zone, 1);
		return EventData("card:play", spellID, copyNum, zone, "faceUp");
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
	Serial.println("Declare an Attack Target!");
}

EventData SpeedDuel::HandleMonsterAttack(String socketID, String zoneName) {
	if (!CheckForValidTarget(zoneName)) {
		Serial.printf("No Targets on Zone: %i\n", zoneName);
		Serial.printf("Hint: Hold Buttons 1 & 5 for a Direct Attack!\n");
		return EventData();
	}

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
void SpeedDuel::UpdateDuelState(String duelistID, int cardID, int copyNumber, String zoneName, int position)
{
	_duelState.UpdateDuelState(duelistID, cardID, copyNumber, zoneName, position);
}

void SpeedDuel::ClearDuelStates() {
	_duelState.ClearPlayerStates();
}

void SpeedDuel::UpdatePhase(String newPhase, bool isOpponentsTurn) {
	_duelState.UpdatePhase(newPhase, isOpponentsTurn);
}

void SpeedDuel::UpdateLifepoints(String lifepoints, String duelistID) {
	char buffer[9];
	lifepoints.toCharArray(buffer, 9);
	int LP = atoi(&buffer[0]);

	_duelState.UpdateLifepoints(LP, duelistID);
}

String SpeedDuel::GetPlayerLP() {
	return "Player: " + String(_duelState.GetPlayerLP());
}

String SpeedDuel::GetOppLP() {
	return "Opp: " + String(_duelState.GetOppLP());
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

String SpeedDuel::GetZoneName(int zone) {
	String zoneName;
	switch (zone) {
		case 0:
			zoneName = "mainMonster1";
			break;
		case 1:
			zoneName = "mainMonster2";
			break;
		case 2:
			zoneName = "mainMonster3";
			break;
		case 3:
			zoneName = "spellTrap1";
			break;
		case 4:
			zoneName = "spellTrap2";
			break;
		case 5:
			zoneName = "spellTrap3";
			break;
		case 7:
			zoneName = "skill";
			break;
	}

	return zoneName;
}