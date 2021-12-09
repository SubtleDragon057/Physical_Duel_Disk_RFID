#ifndef StorageHandler_h
#define StorageHandler_h
#include "Arduino.h"
#include "FS.h"
#include "CommunicationsHandler.h"

class StorageHandler {
private:
	
	String _deckName;
	File _deckYDK;
	CommunicationsHandler* _communicationsHandler;
	String _deckNames[5];

public:
	int DeckList[36];
	bool IsDeckSet = false;

	StorageHandler(CommunicationsHandler &communicatiosnHandler);
	
	void ChooseDeck(int buttonEvents[]);
	String GetDeckName(int buttonEvents[]);
	void GetListOfDecks();
	void ReadYDK(String deckName);
};

#endif