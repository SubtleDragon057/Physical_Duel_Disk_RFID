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
	String comma = ",";
	String eventInfo;
	Zone::Monster monster = zone.GetCurrentMonster();

	eventInfo.concat(eventType);
	eventInfo.concat(comma);

	//Card Zone
	if (isMonster) {
		eventInfo.concat(zone.zoneNumber);
		eventInfo.concat(comma);
	}
	else if (!isMonster) {
		eventInfo.concat(zone.zoneNumber + 3);
		eventInfo.concat(comma);
	}

	//Card ID and Orientation
	if (isMonster) {
		if (monster.GetSerialNumber() == "") {
			eventInfo.concat("00000000");
			eventInfo.concat(comma);
		}
		else {
			eventInfo.concat(monster.GetSerialNumber());
			eventInfo.concat(comma);
		}

		eventInfo.concat(monster.GetMode(Zone::Battle));
		eventInfo.concat(comma);
		eventInfo.concat(monster.GetMode(Zone::Face));
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
		monster.UpdateMonsterMode(Zone::Battle, Zone::Monster::Defence);
		return;
	}

	monster.UpdateMonsterMode(Zone::Battle, Zone::Monster::Attack);
}

void LocalFunctions::UpdateFaceMode(Zone::Monster monster, int pin) {
	int lightValue = analogRead(pin);

	if (lightValue > 300) {
		monster.UpdateMonsterMode(Zone::Face, Zone::Monster::FaceDown);
		return;
	}

	monster.UpdateMonsterMode(Zone::Face, Zone::Monster::FaceUp);
}

LocalFunctions::Events LocalFunctions::SetEventType(Zone zone, bool* isMonster, String cardSerialNumber)
{
	//Changes to Monster Zone
	Zone::Monster monster = zone.GetCurrentMonster();
	if (isMonster) {
		zone.UpdateCurrentMonster(cardSerialNumber);
		if (monster.GetSerialNumber() == "") {
			return Events::Remove;
		}

		return Events::Summon;
	}
	
	//TODO: Update position change function or add to ESP??
	/*else if (isMonster && (newBattleMode || newFaceMode)) {

		eventType = LocalFunctions::PositionChange;
		currentLightValueBattle[slot] = lightValueBattle[slot];
		currentLightValueFace[slot] = lightValueFace[slot];
	}*/
	
	//Changes to Spell/Trap Zone
	if (!isMonster) {
		zone.UpdateCurrentSpell(cardSerialNumber);
		if (zone.GetCurrentSpell() == "") {
			return Events::Remove;
		}

		return Events::SetSpellTrap;
	}

	*isMonster = true;
	zone.UpdateCurrentMonster(cardSerialNumber);
	return Events::Remove;
}

//TODO: Support Spell Removal
String LocalFunctions::RemoveCard(Zone zone)
{
	String comma = ",";
	String eventInfo;
	Zone::Monster monster = zone.GetCurrentMonster();

	eventInfo.concat(Events::Remove);
	eventInfo.concat(comma);

	eventInfo.concat(zone.zoneNumber);
	eventInfo.concat(comma);

	eventInfo.concat("00000000");
	eventInfo.concat(comma);

	eventInfo.concat(Zone::Monster::Modes::Attack);
	eventInfo.concat(comma);
	eventInfo.concat(Zone::Monster::Modes::FaceDown);

	monster.UpdateMonsterSerial("00000000");

	return eventInfo;
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