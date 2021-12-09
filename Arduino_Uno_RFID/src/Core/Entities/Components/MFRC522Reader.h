#ifndef MFRCReader_h
#define MFRCReader_h
#include "Arduino.h"
#include "MFRC522.h"

class mfrcReader : public MFRC522 {
private:
	byte _block;
	MFRC522::MIFARE_Key _key;

public:

	mfrcReader() {}
	mfrcReader(byte readerPin, byte resetPin, byte block);

	void DebugReader();
	bool ScanForNewCard();
	bool ReadAvailableCard();
	void StopScanning();
	int ReadBlock(byte readBackBlock[]);
};

#endif