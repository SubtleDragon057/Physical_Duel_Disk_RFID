#ifndef LocalFunctions_h
#define LocalFunctions_h
#include "Arduino.h"

class LocalFunctions
{
	private:

		String GetZoneName(int zoneNumber);
		String GetCardPosition(String positionNumber);
		String GetCardEvent(String socketID, int eventName, String eventInfo[]);

	public:
		LocalFunctions();	
		
		String GetCardEventAsJSON(String socketID, String data);
		
		String SummonEvent(String socketID, String eventInfo[]);
		String RemoveCardEvent(String socketID, String eventInfo[]);
};

#endif