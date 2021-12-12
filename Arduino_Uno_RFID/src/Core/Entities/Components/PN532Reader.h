#ifndef PN532Reader_h
#define PN532Reader_h
#include "Arduino.h"
#include "Models\NFCReader.h"
#include "PN532.h"
#include "PN532_I2C.h"
#include "Wire.h"

class PN532Reader : public NFCReader {
private:
	byte _block;

	byte _uid[7];
	byte _uidLength;
	byte _key[6];

	PN532 reader;

public:

	PN532Reader() {}
	PN532Reader(TwoWire &Wire, byte block);

	void DebugReader();
	bool ScanForNewCard();
	bool ReadAvailableCard();
	void StopScanning();
	int ReadBlock(byte readBackBlock[]);
};

#endif