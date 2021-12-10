#ifndef ZoneHandler_h
#define ZoneHandler_h
#include "Arduino.h"
#include "Core\Zone.h"
#include "Core\Entities\Enums.h"
#include "PN532.h"

class ZoneHandler {
private:
	byte _block = 4;
	byte _readBackBlock[16];
	
	bool _debug;

	// This is where you put the cards to write to the NFC chips. See NOTE for details
	const byte _cardsToRead = 1;
	int cardNames[1][9] = {
		{1, 6, 6, 6, 7, 2, 5, 6, 9}  //Dragon Zombie
	};

	/*------------------------------------------------------NOTE---------------------------------------------------------------//
	  // Start by changing '_cardsToRead' to the number of cards you plan to add. If you're writing 6 cards, it needs to be 6.
	  // Next, add the card using the 9 digit format inside {curly brackets}, <-- and make sure to end with the comma.
	  // Cards can be obtained from the CARD_INDEX.txt or entered manually. See below.
	  //
	  // To enter cards manually, you will need the serial number from the bottom left of the Yu-gi-oh card. We will use
	  // Silver Fang for this example. It's serial number is 90357090
	  // To start we need to add the first digit depending on the card type: Monser => 1, Spell => 2, Trap => 3 and separate
	  // each number with a comma.
	  //
	  // That means it would look like this 1,9,0,3,5,7,0,9,0  //
	  // Then, we need to put those numbers into the {} so it should look like this: {1,9,0,3,5,7,0,9,0},
	  //----------------------------------------------------END NOTE-------------------------------------------------------------*/

	void CheckRFIDReader(DualCardZone &zone, Enums::SensorType);

public:
		
	DualCardZone Zones[3] = {
		DualCardZone(),
		DualCardZone(),
		DualCardZone()
	};
	
	ZoneHandler(bool debug = false);

	void Initialize(byte numZones, byte readerPins[], byte attackSensorPins[], PN532 reader,
		byte defenceSnesorPins[], byte spellSensorPins[]);
	Enums::SensorType CheckForTrippedSensor(int zoneNumber);
	void HandleUpdateCard(DualCardZone& zone, Enums::SensorType sensor, bool isRemoval = false);
};

#endif