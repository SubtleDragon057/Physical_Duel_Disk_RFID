#ifndef ReadFunctions_h
#define ReadFunctions_h
#include "Arduino.h"
#include "MFRC522.h"

static class Read {
private:
	byte _block;
	MFRC522::MIFARE_Key _key;

public:
	Read();

	void Initialize(byte block);
	void Initialize(byte block, byte customKey[]);
	int ReadBlock(MFRC522 reader, byte readBackBlock[]);
} readStatic;

#endif
