#include "PN532Reader.h"

PN532Reader::PN532Reader()
{
}
PN532Reader::PN532Reader(byte readerPin, byte block)
{
	this->beginSPI(readerPin);
	this->SAMConfig();

	_block = block;

	for (byte i = 0; i < 6; i++) {
		this->_key[i] = 0xFF;
	}
}

// TODO: Move this to helper function in library like MFRC
void PN532Reader::DebugReader()
{
	uint32_t versiondata = this->getFirmwareVersion();
	if (!versiondata) {
		return;
	}

	Serial.print("Firmware ver. "); Serial.print((versiondata >> 16) & 0xFF, DEC);
	Serial.print('.'); Serial.println((versiondata >> 8) & 0xFF, DEC);
}

bool PN532Reader::ScanForNewCard()
{
	return this->readPassiveTargetID(PN532_MIFARE_ISO14443A, this->_uid, &(this->_uidLength));
}

// TODO: Does this need different logic??
bool PN532Reader::ReadAvailableCard()
{
	return true;
}

// TODO: Does this reader need to sleep??
void PN532Reader::StopScanning()
{
	Serial.println("[DEBUG] PN532Reader StopScanning!");
}

int PN532Reader::ReadBlock(byte readBackBlock[])
{
	int trailerBlock = (_block / 4 * 4) + 3;
	byte buffersize = 18;

	uint8_t status = this->mifareclassic_AuthenticateBlock(this->_uid, _uidLength, _block, this->_key);
	if (!status) {
		return 3;
	}

	status = this->mifareclassic_ReadDataBlock(_block, readBackBlock);
	if (!status) {
		Serial.print("[WARN] Could not read block!\n");
		return 4;
	}
}