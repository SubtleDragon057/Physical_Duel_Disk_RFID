#include "Zone.h"
#include "MFRC522.h"

Zone::Zone()
{
}

void Zone::Zone_Init(int zoneNum, MFRC522 mfrc522, ProximitySensor proximitySensor, int battlePin, int facePin)
{
	zoneNumber = zoneNum;
	reader = mfrc522;
	sensor = proximitySensor;
	battleLightPin = battlePin;
	faceLightPin = facePin;

	_currentMonster = Monster("", 0, 0);
}

void Zone::UpdateCurrentMonster(String monsterID)
{
	_currentMonster.UpdateMonsterSerial(monsterID);
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

void Zone::ProximitySensor::ProximitySensor_Init(byte pinNumber)
{
	pin = pinNumber;
	pinMode(pin, INPUT);

	_currentValue = HIGH;
}

bool Zone::ProximitySensor::CheckForNewCard()
{
	int read = digitalRead(pin);

	if (read == _currentValue) {
		return false;
	}

	_currentValue = read;
	return true;
}

Zone::Monster::Monster()
{
}

Zone::Monster::Monster(String serialNumber, int battleMode, int faceMode)
{
	_serialNumber = serialNumber;
	_battleMode = battleMode;
	_faceMode = faceMode;
}

String Zone::Monster::GetSerialNumber()
{
	return _serialNumber;
}

int Zone::Monster::GetMode(Zone::LightValue mode)
{
	switch (mode) {
	case LightValue::Battle:
		return _battleMode;
		break;
	case LightValue::Face:
		return _faceMode;
		break;
	}
}

void Zone::Monster::UpdateMonsterSerial(String serialNumber)
{
	_serialNumber = serialNumber;
}

void Zone::Monster::UpdateMonsterMode(LightValue mode, Modes newMode)
{
	switch (mode) {
	case LightValue::Battle:
		_battleMode = newMode;
		break;
	case LightValue::Face:
		_faceMode = newMode;
		break;
	}
}
