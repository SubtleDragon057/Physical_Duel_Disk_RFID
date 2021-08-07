#ifndef LocalFunctions_h
#define LocalFunctions_h
#include "Arduino.h"

class LocalFunctions
{
	private:
		int* _decklist;
		String _customID;

		String GetZoneName(int zoneNumber);
		String GetCardPosition(String eventInfo[]);
		String GetCardEvent(int eventName, String eventInfo[]);

	public:
		LocalFunctions();
		void Begin(int deckList[], String customID);		
		
		String GetCardEventAsJSON(String data);
		
		String CreateRoom();
		String JoinRoom(String roomName);
		String SummonEvent(String eventInfo[]);
		String RemoveCardEvent(String eventInfo[]);
};

#endif