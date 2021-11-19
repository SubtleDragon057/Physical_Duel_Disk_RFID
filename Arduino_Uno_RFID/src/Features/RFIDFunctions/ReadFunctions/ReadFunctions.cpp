#include "Arduino.h"
#include "ReadFunctions.h"
#include "MFRC522.h"

Read::Read()
{
}

void Read::Initialize(byte block)
{
	_block = block;

	for (byte i = 0; i < 6; i++) {
		_key.keyByte[i] = 0xFF;
	}
}
void Read::Initialize(byte block, byte customKey[])
{
	_block = block;

	for (byte i = 0; i < 6; i++) {
		_key.keyByte[i] = customKey[i];
	}
}

int Read::ReadBlock(MFRC522 reader, byte readBackBlock[])
{
	int largestModulo4Number = _block / 4 * 4;
	int trailerBlock = largestModulo4Number + 3;
	byte buffersize = 18;

	byte status = reader.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &_key, &(reader.uid));
	if (status != MFRC522::STATUS_OK) {
		return 3;
	}

	status = reader.MIFARE_Read(_block, readBackBlock, &buffersize);
	if (status != MFRC522::STATUS_OK) {
		return 4;
	}
}