#ifndef Components_h
#define Components_h
#include "Arduino.h"
#include "Enums.h"
#include "MFRC522.h"

class ProximitySensor {
protected:
	uint8_t _pin;
	int _currentValue;
	String _name;

public:

	ProximitySensor();
	ProximitySensor(uint8_t pin, String name);
	virtual bool isNewCardPresent();
	virtual int GetCurrentValue();
};

class AnalogIR : public ProximitySensor {
private:
	int GetSensorReading();
	bool _debug;

public:
	
	AnalogIR();
	AnalogIR(uint8_t pin, String name, bool debug);
	
	int GetCurrentValue() override;
	bool isNewCardPresent() override;
};

class nfcReader {
public:
	
	nfcReader();

	virtual void DebugReader() {};
	virtual bool ScanForNewCard() {};
	virtual bool ReadAvailableCard() {};
	virtual void StopScanning() {};
	virtual int ReadBlock(byte readBackBlock[]) {};
};

class mfrcReader : public nfcReader {
private:
	MFRC522 _mfrc522;
	byte _block = 4; // TODO: move this to initialize
	MFRC522::MIFARE_Key _key;

public:

	mfrcReader(byte readerPin, byte resetPin);

	void DebugReader() override;
	bool ScanForNewCard() override;
	bool ReadAvailableCard() override;
	void StopScanning() override;
	int ReadBlock(byte readBackBlock[]) override;
};

class pnReader : public nfcReader {
public:
	pnReader(byte readerPin, byte resetPin);

	void DebugReader() override;
	bool ScanForNewCard() override;
	bool ReadAvailableCard() override;
	void StopScanning() override;
	int ReadBlock(byte readBackBlock[]) override;
};

#endif