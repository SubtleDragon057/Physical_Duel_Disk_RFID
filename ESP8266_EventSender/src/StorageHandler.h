#ifndef StorageHandler_h
#define StorageHandler_h
#include "Arduino.h"
#include "FS.h"
#include "SD.h"
#include "SPI.h"

class StorageHandler {
private:
	
	String _deckName;
	File _deckYDK;

	String _deckNames[10];
	
	bool _debug;

public:
	int DeckList[36];
	bool IsDeckSet = false;

	StorageHandler(bool debug);
	
	void ChooseDeck(int buttonEvents[]);
	String GetDeckName(int buttonEvents[]);
	void GetListOfDecks();
	void ReadYDK(String deckName);
};

#endif