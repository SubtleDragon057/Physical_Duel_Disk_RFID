#ifndef _READWRITEFUNCTIONS_h
#define _READWRITEFUNCTIONS_h
#include "arduino.h"
#include "MFRC522.h"

class Read {
private:
	byte _block;
	MFRC522::MIFARE_Key _key;

public:
	Read();

	void Initialize(byte block);
	void Initialize(byte block, byte customKey[]);
	int ReadBlock(MFRC522 reader, byte readBackBlock[]);
};

class Write {
private:
	byte _block;
	MFRC522::MIFARE_Key _key;
	int _cardsToRead;

	void VisualDelay(int halfSeconds);
	String WaitForNewCard(MFRC522 reader);
	int WriteBlock(MFRC522 reader, byte cardID[]);

public:
	Write();

	void Initialize(byte block, int cardsToRead);
	void Initialize(byte block, int cardsToRead, byte customKey[]);
	void ActivateWriteMode(MFRC522 reader, int cardNames[][9]);
};

#endif