#include "Arduino.h"
#include "RFIDFunctions.h"
#include "MFRC522.h"
#include "ReadFunctions/ReadFunctions.h"
#include "WriteFunctions/WriteFunctions.h"

RFID_Functions::RFID_Functions()
{
}

void RFID_Functions::Initialize(byte block, int cardsToRead)
{	
	_readFunctions.Initialize(block);
	_writeFunctions.Initialize(block, cardsToRead);
}

void RFID_Functions::Initialize(byte block, byte customKey[], int cardsToRead)
{
	_readFunctions.Initialize(block, customKey);
	_writeFunctions.Initialize(block, cardsToRead, customKey);
}

int RFID_Functions::ReadBlock(MFRC522 reader, byte readBackBlock[])
{
	return _readFunctions.ReadBlock(reader, readBackBlock);
}

void RFID_Functions::ActivateWriteMode(MFRC522 reader, int cardNames[][9])
{	
	_writeFunctions.ActivateWriteMode(reader, cardNames);
}
