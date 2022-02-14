#include "PN532Reader.h"


PN532Reader::PN532Reader(TwoWire &Wire, byte block)
{
	PN532_I2C i2c(Wire);
	reader = PN532(i2c);
	
	reader.SAMConfig();

	_block = block;

	for (byte i = 0; i < 6; i++) {
		_key[i] = 0xFF;
	}
}

// TODO: Move this to helper function in library like MFRC
void PN532Reader::DebugReader()
{
	uint32_t versiondata = reader.getFirmwareVersion();
	if (!versiondata) {
		return;
	}

	Serial.print("Firmware ver. "); Serial.print((versiondata >> 16) & 0xFF, DEC);
	Serial.print('.'); Serial.println((versiondata >> 8) & 0xFF, DEC);
}

bool PN532Reader::ScanForNewCard()
{
	return reader.readPassiveTargetID(PN532_MIFARE_ISO14443A, _uid, &_uidLength);
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

	uint8_t status = reader.mifareclassic_AuthenticateBlock(_uid, _uidLength, _block, 0, _key);
	if (!status) {
		return 3;
	}

	status = reader.mifareclassic_ReadDataBlock(_block, readBackBlock);
	if (!status) {
		Serial.print("[WARN] Could not read block!\n");
		return 4;
	}
}