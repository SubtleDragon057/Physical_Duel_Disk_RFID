#ifndef LocalFunctions_h
#define LocalFunctions_h
#include "Arduino.h"

class LocalFunctions
{
	private:
		int* _decklist;

		String GetZoneName(int zoneNumber);
		String GetCardPosition(String eventInfo[]);
		String GetCardEvent(String socketID, int eventName, String eventInfo[]);

	public:
		LocalFunctions();
		void Initialize(int deckList[]);		
		
		String GetCardEventAsJSON(String socketID, String data);
		
		String CreateRoom();
		String JoinRoom(String roomName);
		String SummonEvent(String socketID, String eventInfo[]);
		String RemoveCardEvent(String socketID, String eventInfo[]);
		String RecieveEvent(uint8_t* payload);
};

#endif