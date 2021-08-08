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
		NoEvent,
		Summon,
		Remove,
		PositionChange,
		SetSpellTrap,
		ActivateSpellTrap,
	};

	LocalFunctions();
	bool HasModeChanged(Zone zone, Zone::LightValue value);
	bool CheckIfCardIsMonster(byte readBackBlock);
	void CheckBattleAndFaceModes(Zone zone);
	String FormatEventInfo(Zone zone, LocalFunctions::Events eventType, bool isMonster);
	LocalFunctions::Events SetEventType(Zone zone, bool* isMonster, String cardSerialNumber);
};

#endif