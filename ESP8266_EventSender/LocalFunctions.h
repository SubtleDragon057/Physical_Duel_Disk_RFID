#ifndef LocalFunctions_h
#define LocalFunctions_h
#include "Arduino.h"

class LocalFunctions
{
	private:
		int* _decklist;

	public:
		LocalFunctions();
		void Begin(int deckList[]);

		String GetCardPosition(String eventInfo[]);
		String GetZoneName(int zoneNumber);
		
		String SummonEvent(String eventInfo[]);
		String RemoveCardEvent(String eventInfo[]);
		String CreateRoom();
};

#endif