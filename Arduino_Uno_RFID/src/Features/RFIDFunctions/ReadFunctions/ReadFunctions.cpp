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
	int trailerBlock = (_block / 4 * 4) + 3;
	byte buffersize = 18;

	MFRC522::StatusCode status = reader.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &_key, &(reader.uid));
	if (status != MFRC522::STATUS_OK) {
		Serial.print("[WARN] Could not authenticate read!\nStatus code: ");
		Serial.println(reader.GetStatusCodeName(status));
		return 3;
	}

	status = reader.MIFARE_Read(_block, readBackBlock, &buffersize);
	if (status != MFRC522::STATUS_OK) {
		Serial.print("[WARN] Could not read block!\nStatus code: ");
		Serial.println(status);
		return 4;
	}
}