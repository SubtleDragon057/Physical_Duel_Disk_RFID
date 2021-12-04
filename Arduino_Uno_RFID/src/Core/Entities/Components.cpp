#include "Arduino.h"
#include "Components.h"
#include "Enums.h"
#include "MFRC522.h"

ProximitySensor::ProximitySensor() 
{
}
ProximitySensor::ProximitySensor(uint8_t pin, String name) {
	_pin = pin;
	_currentValue = HIGH;
	_name = name;

	pinMode(_pin, INPUT);
}

bool ProximitySensor::isNewCardPresent() {
	int read = digitalRead(_pin);

	if (read == _currentValue) {
		return false;
	}

	_currentValue = read;
	return true;
}

int ProximitySensor::GetCurrentValue() {
	_currentValue = digitalRead(_pin);
	return _currentValue;
}

AnalogIR::AnalogIR() {

}
AnalogIR::AnalogIR(uint8_t pin, String name, bool debug) {
	_pin = pin;
	_currentValue = Enums::High;
	_name = name;
	_debug = debug;
}

int AnalogIR::GetCurrentValue()
{	
	_currentValue = GetSensorReading();
	return _currentValue;
}

bool AnalogIR::isNewCardPresent() {
	int read = GetSensorReading();
	
	if (_debug) {
		Serial.print("Current Value: ");
		Serial.println(_currentValue);
		Serial.print("Read Value: ");
		Serial.println(read);
		delay(50);
	}

	if (read == _currentValue) {
		return false;
	}

	_currentValue = read;
	return true;
}

int AnalogIR::GetSensorReading() {
	int read = analogRead(_pin);
	
	if (_debug) {
		Serial.print("Name: ");
		Serial.println(_name);
		Serial.print("Analog Val: ");
		Serial.println(read);
		delay(50);
	}

	if (read < 100) {
		return Enums::Low;
	}
	else if (read > 550) {		
		return Enums::High;
	}
	
	return Enums::Medium;
}

nfcReader::nfcReader()
{
}

mfrcReader::mfrcReader(byte readerPin, byte resetPin)
{
	_mfrc522.PCD_Init(readerPin, resetPin);
	_mfrc522.PCD_SetAntennaGain(MFRC522::PCD_RxGain::RxGain_avg);
}

void mfrcReader::DebugReader()
{
	_mfrc522.PCD_DumpVersionToSerial();
	/*Serial.println(F("[DEBUG] Performing test..."));
	bool result = _mfrc522.PCD_PerformSelfTest();
	Serial.print(F("[DEBUG] Result: "));
	if (result)
		Serial.println(F("OK"));
	else
		Serial.println(F("DEFECT or UNKNOWN"));*/
	Serial.println();
}

bool mfrcReader::ScanForNewCard()
{
	Serial.println("[DEBUG] MFRC ReadBlock Entered");

	_mfrc522.PCD_Init();
	return _mfrc522.PICC_IsNewCardPresent();
}

bool mfrcReader::ReadAvailableCard()
{
	Serial.println("[DEBUG] MFRC ReadBlock Entered");
	
	return _mfrc522.PICC_ReadCardSerial();
}

void mfrcReader::StopScanning()
{
	Serial.println("[DEBUG] MFRC ReadBlock Entered");
	
	_mfrc522.PICC_HaltA();
	_mfrc522.PCD_StopCrypto1();
}

int mfrcReader::ReadBlock(byte readBackBlock[])
{
	Serial.println("[DEBUG] MFRC ReadBlock Entered");
	
	int trailerBlock = (_block / 4 * 4) + 3;
	byte buffersize = 18;

	MFRC522::StatusCode status = _mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &_key, &(_mfrc522.uid));
	if (status != MFRC522::STATUS_OK) {
		Serial.print("[WARN] Could not authenticate read!\nStatus code: ");
		Serial.println(_mfrc522.GetStatusCodeName(status));
		return 3;
	}

	status = _mfrc522.MIFARE_Read(_block, readBackBlock, &buffersize);
	if (status != MFRC522::STATUS_OK) {
		Serial.print("[WARN] Could not read block!\nStatus code: ");
		Serial.println(status);
		return 4;
	}
}

pnReader::pnReader(byte readerPin, byte resetPin)
{
}

void pnReader::DebugReader()
{
	Serial.println("[DEBUG] pnReader DebugReader!");
}

bool pnReader::ScanForNewCard()
{
	Serial.println("[DEBUG] pnReader ScanForNewCard!");
	return false;
}

bool pnReader::ReadAvailableCard()
{
	Serial.println("[DEBUG] pnReader ReadAvailableCard!");
	return false;
}

void pnReader::StopScanning()
{
	Serial.println("[DEBUG] pnReader StopScanning!");
}

int pnReader::ReadBlock(byte readBackBlock[])
{
	Serial.println("[DEBUG] pnReader ReadBlock!");
	return 0;
}
