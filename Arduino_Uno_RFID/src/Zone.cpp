#include "Zone.h"
#include "MFRC522.h"

Zone::Zone()
{
}

Zone::Zone(int zoneNum, MFRC522 mfrc522, ProximitySensor proximitySensor, int battlePin, int facePin)
{
	zoneNumber = zoneNum;
	reader = mfrc522;
	sensor = proximitySensor;
	battleLightPin = battlePin;
	faceLightPin = facePin;
}

bool Zone::CheckForNewCard()
{
	int read = digitalRead(sensor.pin);

	if (read != sensor.currentValue) {
		sensor.currentValue = read;
		return true;
	}

	return false;
}

void Zone::UpdateCurrentMonster(String monsterID)
{
	_currentMonster.serialNumber = monsterID;
}

void Zone::UpdateCurrentSpell(String spellID)
{
	_currentSpell = spellID;
}

Zone::Monster Zone::GetCurrentMonster()
{
	return _currentMonster;
}

String Zone::GetCurrentSpell()
{
	return _currentSpell;
}

int Zone::GetLightValue(LightValue value)
{
	switch (value) {
	case Zone::Battle:
		return _currentBattleLightValue;
		break;
	case Zone::Face:
		return _currentFaceLightValue;
		break;
	}
}

Zone::ProximitySensor::ProximitySensor()
{
}

Zone::ProximitySensor::ProximitySensor(byte pinNumber)
{
	pin = pinNumber;
}

Zone::Monster::Monster()
{
}