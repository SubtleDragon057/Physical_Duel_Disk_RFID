#ifndef DuelRoom_h
#define DuelRoom_h
#include "Arduino.h"

class DuelRoom {
private:
	String HandleCloseRoomEvent();

public:
	String RoomName;
	
	DuelRoom();

	String CheckDuelRoomForAction(int buttonEvents[]);
	//void UpdateCurrentRoom(String roomName);
};

#endif