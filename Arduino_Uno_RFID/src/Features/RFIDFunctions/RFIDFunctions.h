#ifndef RFIDFunctions_h
#define RFIDFunctions_h
#include "Arduino.h"
#include "MFRC522.h"
#include "ReadFunctions/ReadFunctions.h"
#include "WriteFunctions/WriteFunctions.h"

class RFID_Functions {
	Read _readFunctions = readStatic;
	Write _writeFunctions = writeStatic;

public:

	RFID_Functions();

	// Initialization
	void Initialize(byte block, int cardsToRead);
	void Initialize(byte block, byte customKey[], int cardsToRead);
	
	// Read Functions
	int ReadBlock(MFRC522 reader, byte readBackBlock[]);

	// Write Functions
	void ActivateWriteMode(MFRC522 reader, int cardNames[][9]);
};

#endif
