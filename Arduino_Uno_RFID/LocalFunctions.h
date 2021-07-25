#ifndef LocalFunctions_h
#define LocalFunctions_h
#include "Arduino.h"
#include "MFRC522.h"

class LocalFunctions
{
	private:		
		MFRC522::MIFARE_Key _key;
		int _block;
		int _cardsToRead;

		String WaitForNewCard(MFRC522 reader);
		void VisualDelay(int halfSeconds);

	public:
		LocalFunctions();
		void InitializeWriteFeatures(MFRC522::MIFARE_Key key, int block, int cardsToRead);
		bool CheckMode(int value[], byte pin[], int slot, int current[], int cardMode[]);
		void WriteModeActivated(MFRC522 reader, int cardNames[][9]);
		int WriteBlock(MFRC522 reader, byte cardID[]);
		int ReadBlock(MFRC522 reader, byte readBackBlock[]);
		void DebugRFIDConnection(int i, byte ssPins[], MFRC522 cardSlots[]);
};

#endif