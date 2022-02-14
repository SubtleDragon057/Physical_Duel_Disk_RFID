#ifndef NFCReader_h
#define NFCReader_h
#include "Arduino.h"

class NFCReader {
public:
	virtual void DebugReader() = 0;
	virtual bool ScanForNewCard() = 0;
	virtual bool ReadAvailableCard() = 0;
	virtual void StopScanning() = 0;
	virtual int ReadBlock(byte readBackBlock[]) = 0;
};

#endif