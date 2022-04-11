#ifndef PeripheralsHandler_h
#define PeripheralsHandler_h
#include "arduino.h"
#include "Core\WifiManager.h"
#include "Features\DisplayManager.h"
#include "Secrets.h"
#include "Core\Entities\Enums.h"

class PeripheralsHandler {
private:
	WifiManager _wifiManager;
	DisplayManager _displayManager;
	SECRETS _secrets;

#ifdef ESP8266;
	const byte _muxPin1 = 0;
	const byte _muxPin2 = 2;
	const byte _muxPin3 = 14;
	const byte _muxPin4 = 12;
#endif
#ifdef ESP32
	const byte _muxPin1 = 32;
	const byte _muxPin2 = 33;
	const byte _muxPin3 = 25;
	const byte _muxPin4 = 26;
#endif

	const byte _multiplexerAddress = 0x70;
	const byte _muxPins[4] = {
		_muxPin1,
		_muxPin2,
		_muxPin3,
		_muxPin4
	};
	const byte _muxChannels[16][4] = {
		{0,0,0,0}, //channel 0
		{1,0,0,0}, //channel 1
		{0,1,0,0}, //channel 2
		{1,1,0,0}, //channel 3
		{0,0,1,0}, //channel 4
		{1,0,1,0}, //channel 5
		{0,1,1,0}, //channel 6
		{1,1,1,0}, //channel 7
		{0,0,0,1}, //channel 8
		{1,0,0,1}, //channel 9
		{0,1,0,1}, //channel 10
		{1,1,0,1}, //channel 11
		{0,0,1,1}, //channel 12
		{1,0,1,1}, //channel 13
		{0,1,1,1}, //channel 14
		{1,1,1,1}  //channel 15
	};

public:
	typedef enum {
		UI_Init,
		UI_DeckSelect,
		UI_WriteMode,
		UI_Lobby,
		UI_DuelRoom,
		UI_SpeedDuel,
		UI_ConnectionError
	} UI_Type;

	volatile bool IsBladeConnected = false;
	
	PeripheralsHandler();

	void InitializeCommunications();
	void EnableWriteMode();
	void TransmitCard(String cardNumber);
	void Display(UI_Type type, String incomingMessage[]);
	void SelectMultiplexerAddress(uint8_t address);
};

#endif

