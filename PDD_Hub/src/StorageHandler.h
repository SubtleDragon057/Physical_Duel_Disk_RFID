#ifndef StorageHandler_h
#define StorageHandler_h
#include "Arduino.h"
#include "FS.h"
#include "PeripheralsHandler.h"

class StorageHandler {
private:

	bool _isSDConnected;
	String _deckName;
	File _deckYDK;
	PeripheralsHandler* _peripheralsHandler;
	String _deckNames[5];

	int _defaultDeckList[35] = {
		300101004,
		25652259,
		25652259,
		64788463,
		90876561,
		71413901,
		89631139,
		11321183,
		45141844,
		30860696,
		88472456,
		34627841,
		24094653,
		24094653,
		32807846,
		32807846,
		95281259,
		14087893,
		68005187,
		25005816,
		73729209,
		6150044
		};

public:
	int DeckList[36];
	bool IsDeckSet = false;

	StorageHandler(PeripheralsHandler &peripheralsHandler);

	void Initialize(bool isSDReaderConnected);
	
	void ChooseDeck(int buttonEvents[]);
	void UseDefaultDeck();
	String GetDeckName(int buttonEvents[]);
	void GetListOfDecks();
	void ReadYDK(String deckName);
};

#endif