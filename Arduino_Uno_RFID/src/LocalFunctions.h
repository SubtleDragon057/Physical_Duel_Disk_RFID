#ifndef LocalFunctions_h
#define LocalFunctions_h
#include "Arduino.h"
#include "MFRC522.h"
#include "Zone.h"

class LocalFunctions {
private:
	int GetCurrentPin(Zone zone, Zone::LightValue value);
	void LocalFunctions::UpdateBattleMode(Zone::Monster monster, int pin);
	void LocalFunctions::UpdateFaceMode(Zone::Monster monster, int pin);

public:
	enum Events {
		NoEvent = 0,
		Summon = 1,
		Remove = 2,
		PositionChange = 3,
		SetSpellTrap = 4,
		ActivateSpellTrap = 5,
	};

	LocalFunctions();
	bool HasModeChanged(Zone zone, Zone::LightValue value);
	bool CheckIfCardIsMonster(byte readBackBlock);
	void CheckBattleAndFaceModes(Zone zone);
	String FormatEventInfo(Zone zone, LocalFunctions::Events eventType, bool isMonster);
	Events SetEventType(Zone zone, bool* isMonster, String cardSerialNumber);
	String RemoveCard(Zone zone);
};

#endif