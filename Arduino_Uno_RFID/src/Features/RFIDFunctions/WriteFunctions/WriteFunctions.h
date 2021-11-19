#ifndef WriteFunctions_h
#define WriteFunctions_h
#include "Arduino.h"
#include "MFRC522.h"

static class Write {
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
} writeStatic;

#endif