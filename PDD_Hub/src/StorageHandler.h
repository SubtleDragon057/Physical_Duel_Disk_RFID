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
		25652259,
		25652259,
		11549357,
		90876561,
		62651957,
		84636823,
		65622692,
		98502113,
		5818798,
		11321183,
		14898066,
		71413901,
		64500000,
		99785935,
		46986414,
		76909279,
		24094653,
		24094653,
		4796100,
		78193831,
		89631139,
		46363422,
		31553716,
		39256679,
		91998119,
		75347539,
		77207191
		};

public:
	int DeckList[36];
	bool IsDeckSet = false;

	StorageHandler(PeripheralsHandler &communicatiosnHandler);

	void Initialize(bool isSDReaderConnected);
	
	void ChooseDeck(int buttonEvents[]);
	void UseDefaultDeck();
	String GetDeckName(int buttonEvents[]);
	void GetListOfDecks();
	void ReadYDK(String deckName);
};

#endif