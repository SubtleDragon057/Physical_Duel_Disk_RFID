#ifndef StorageHandler_h
#define StorageHandler_h
#include "Arduino.h"
#include "FS.h"

class StorageHandler {
private:
	
	String _deckName;
	File _deckYDK;

	String _deckNames[10];

public:
	int DeckList[36];
	bool IsDeckSet = false;

	StorageHandler();
	
	void ChooseDeck(int buttonEvents[]);
	String GetDeckName(int buttonEvents[]);
	void GetListOfDecks();
	void ReadYDK(String deckName);
};

#endif