#include "Arduino.h"
#include "LocalFunctions.h"
#include "MFRC522.h"
#include "Zone.h"

LocalFunctions::LocalFunctions()
{
}

bool LocalFunctions::HasModeChanged(Zone zone, Zone::LightValue value) {
	
	int currentPin = GetCurrentPin(zone, value);

	int lightValue = analogRead(currentPin);
	if (lightValue == zone.GetLightValue(value)) {
		return false;
	}

	return true;
}

bool LocalFunctions::CheckIfCardIsMonster(byte readBackBlock)
{
	if (readBackBlock != 50 && readBackBlock != 51) {
		return true;
	}
	return false;
}

void LocalFunctions::CheckBattleAndFaceModes(Zone zone) {
	Zone::Monster monster = zone.GetCurrentMonster();

	//Check for Attack or Defence mode
	if (HasModeChanged(zone, Zone::Battle)) {
		UpdateBattleMode(monster, zone.battleLightPin);
	}

	//Check if card is Face Up, or Face Down
	if (HasModeChanged(zone, Zone::Face)) {
		UpdateFaceMode(monster, zone.faceLightPin);
	}
}

String LocalFunctions::FormatEventInfo(Zone zone, LocalFunctions::Events eventType, bool isMonster)
{
	String eventInfo;
	Zone::Monster monster = zone.GetCurrentMonster();

	eventInfo.concat(eventType + ',');

	//Card Zone
	if (isMonster) {
		eventInfo.concat(zone.zoneNumber + ',');
	}
	else if (!isMonster) {
		eventInfo.concat((zone.zoneNumber + 3) + ',');
	}

	//Card ID and Orientation
	if (isMonster) {
		if (monster.serialNumber == "") {
			eventInfo.concat("00000000" + ',');
		}
		else {
			eventInfo.concat(monster.serialNumber + ',');
		}

		eventInfo.concat(monster.battleMode + ',' + monster.faceMode);
	}
	else if (!isMonster) {
		if (zone.GetCurrentSpell() == "") {
			eventInfo.concat("00000000" + ',');
		}
		else {
			eventInfo.concat(zone.GetCurrentSpell() + ',');
		}

		//Spell Cards default to Face Down. Buttons will be used to 'Activate' them...eventually
		eventInfo.concat(Zone::Monster::FaceDown); 
	}
	
	return eventInfo;
}

void LocalFunctions::UpdateBattleMode(Zone::Monster monster, int pin) {
	int lightValue = analogRead(pin);

	if (lightValue > 300) {
		monster.battleMode = Zone::Monster::Defence;
		return;
	}

	monster.battleMode = Zone::Monster::Attack;
}

void LocalFunctions::UpdateFaceMode(Zone::Monster monster, int pin) {
	int lightValue = analogRead(pin);

	if (lightValue > 300) {
		monster.faceMode = Zone::Monster::FaceDown;
		return;
	}

	monster.faceMode = Zone::Monster::FaceUp;
}

LocalFunctions::Events LocalFunctions::SetEventType(Zone zone, bool* isMonster, String cardSerialNumber)
{
	//Changes to Monster Zone
	Zone::Monster monster = zone.GetCurrentMonster();
	if (isMonster && cardSerialNumber != monster.serialNumber) {

		zone.UpdateCurrentMonster(cardSerialNumber);
		if (monster.serialNumber == "") {
			return LocalFunctions::Remove;
		}

		return LocalFunctions::Summon;
	}
	//TODO: Update position change function or add to ESP??
	/*else if (isMonster && (newBattleMode || newFaceMode)) {

		eventType = LocalFunctions::PositionChange;
		currentLightValueBattle[slot] = lightValueBattle[slot];
		currentLightValueFace[slot] = lightValueFace[slot];
	}*/
	//Changes to Spell/Trap Zone
	else if (!isMonster && cardSerialNumber != zone.GetCurrentSpell()) {

		zone.UpdateCurrentSpell(cardSerialNumber);
		if (zone.GetCurrentSpell() == "") {
			return LocalFunctions::Remove;
		}

		return LocalFunctions::SetSpellTrap;
	}

	//Override constant spell bool to allow monster to disappear when card not present	
	if (cardSerialNumber != monster.serialNumber) {

		*isMonster = true;
		zone.UpdateCurrentMonster(cardSerialNumber);
		return LocalFunctions::Remove;
	}
}

int LocalFunctions::GetCurrentPin(Zone zone, Zone::LightValue value) {
	switch (value) {
	case Zone::Battle:
		return zone.battleLightPin;
		break;
	case Zone::Face:
		return zone.faceLightPin;
		break;
	}
}