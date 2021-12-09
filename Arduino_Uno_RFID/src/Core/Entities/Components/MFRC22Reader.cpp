#include "MFRC522Reader.h"
#include "Arduino.h"
#include "MFRC522.h"

mfrcReader::mfrcReader(byte readerPin, byte resetPin, byte block)
{
	this->PCD_Init(readerPin, resetPin);
	this->PCD_SetAntennaGain(MFRC522::PCD_RxGain::RxGain_avg);

	_block = block;

	for (byte i = 0; i < 6; i++) {
		_key.keyByte[i] = 0xFF;
	}
}

void mfrcReader::DebugReader()
{
	this->PCD_DumpVersionToSerial();
	Serial.println();
}

bool mfrcReader::ScanForNewCard()
{
	this->PCD_Init();
	return this->PICC_IsNewCardPresent();
}

bool mfrcReader::ReadAvailableCard()
{
	return this->PICC_ReadCardSerial();
}

void mfrcReader::StopScanning()
{
	this->PICC_HaltA();
	this->PCD_StopCrypto1();
}

int mfrcReader::ReadBlock(byte readBackBlock[])
{
	Serial.println("Read Back Block");
	
	int trailerBlock = (_block / 4 * 4) + 3;
	byte buffersize = 18;

	MFRC522::StatusCode status = this->PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &_key, &(this->uid));
	if (status != MFRC522::STATUS_OK) {
		return 3;
	}

	status = this->MIFARE_Read(_block, readBackBlock, &buffersize);
	if (status != MFRC522::STATUS_OK) {
		Serial.print("[WARN] Could not read block!\nStatus code: ");
		Serial.println(status);
		return 4;
	}
}