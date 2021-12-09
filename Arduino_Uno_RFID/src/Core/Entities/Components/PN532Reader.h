#ifndef PN532Reader_h
#define PN532Reader_h
#include "Arduino.h"
#include "Models\NFCReader.h"
#include "Adafruit_PN532.h"

class PN532Reader : public NFCReader, public Adafruit_PN532 {
private:
	byte _block;

public:

	PN532Reader();
	PN532Reader(byte readerPin, byte block);

	void DebugReader();
	bool ScanForNewCard();
	bool ReadAvailableCard();
	void StopScanning();
	int ReadBlock(byte readBackBlock[]);
};

#endif