#ifndef DuelRoom_h
#define DuelRoom_h
#include "Arduino.h"
#include "ArduinoJson.h"
#include "Lobby.h"

class DuelRoom {
private:
	String HandleCloseRoomEvent();

public:
	DuelRoom();

	String CheckDuelRoomForAction(int buttonEvents[]);
};

#endif