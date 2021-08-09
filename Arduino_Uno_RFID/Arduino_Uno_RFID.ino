/*
  Name:    DuelDisk_ProjectATEM_Demo.ino
  Created: 1/22/2021 11:07:28 AM
  Author:  SubtleDragon057
*/

#include <SPI.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>

#include "src\LocalFunctions.h"
#include "src\ReadWriteFunctions.h"
#include "src\Zone.h"

//---------------------------------------------------------------------------------//
//----------------------------------Globals----------------------------------------//
//---------------------------------------------------------------------------------//

SoftwareSerial toESP(0, 1);
LocalFunctions func;
Read read;
Write write;

//RFID Readers
const byte numReaders = 3;
const byte ssPins[] = { 2, 3, 4 };
const byte resetPin = 8;
MFRC522 cardSlots[numReaders];
Zone::ProximitySensor sensors[numReaders];

const byte sensorPins[] = { 5, 6, 7 };
byte lightPinBattle[] = { A0, A1, A2 };
byte lightPinFace[] = { A3, A4, A5 };

Zone zones[numReaders];

//Spell/Trap
byte spellButton1 = A1;
byte spellButton2 = A2;
byte spellButton3 = A3;

//Info for Writing & Reading the blocks
byte block = 4;
byte readBackBlock[16];

//This is where you put the cards to write to the NFC chips. See NOTE for details
const byte cardsToRead = 1;
int cardNames[cardsToRead][9] = {
	{1, 6, 6, 6, 7, 2, 5, 6, 9}  //Dragon Zombie
};

/*------------------------------------------------------NOTE---------------------------------------------------------------//
  // Start by changing 'cardsToRead' to the number of cards you plan to add. If you're writing 6 cards, it needs to be 6.
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

void setup() {

	Serial.begin(9600); 
	SPI.begin();
	toESP.begin(9600);

	for (int i = 0; i < numReaders; i++) {
		InitializeZones(i);
	}

	//set up Spell/Trap activation buttons
	pinMode(spellButton1, INPUT_PULLUP);
	pinMode(spellButton2, INPUT_PULLUP);
	pinMode(spellButton3, INPUT_PULLUP);

	read.Initialize(block);
	write.Initialize(block, cardsToRead);

	Serial.println();
}


void loop() {

	CheckForWriteMode();

	//Cycle through each zone on the Duel Disk to check for any changes
	for (int i = 0; i < numReaders; i++) {

		bool isThereANewCard = zones[i].sensor.CheckForNewCard();
		if (isThereANewCard) {
			//delay(500);
			CheckRFIDReader(zones[i]);
		}
	}
}

void InitializeZones(int zoneNumber) {
	int zoneNum = zoneNumber;

	cardSlots[zoneNumber].PCD_Init(ssPins[zoneNumber], resetPin);
	cardSlots[zoneNumber].PCD_SetAntennaGain(MFRC522::PCD_RxGain::RxGain_avg);

	sensors[zoneNumber].ProximitySensor_Init(sensorPins[zoneNumber]);

	zones[zoneNumber].Zone_Init(zoneNum, cardSlots[zoneNumber], sensors[zoneNum], lightPinBattle[zoneNumber], lightPinFace[zoneNumber]);
	Zone zone = zones[zoneNumber];

	delay(10);
}

void CheckForWriteMode() {
	if (digitalRead(spellButton1) == LOW && digitalRead(spellButton3) == LOW) {
		delay(5000);
		if (digitalRead(spellButton1) == LOW && digitalRead(spellButton3) == LOW) {
			write.ActivateWriteMode(cardSlots[1], cardNames);
			return;
		}

		Serial.println("buttons released too early");
	}
}

void CheckRFIDReader(Zone zone) {
	bool isCardPresent;
	bool isMonster = false;
	LocalFunctions::Events eventType = LocalFunctions::NoEvent;

	zone.reader.PCD_Init();
	
	if (!zone.reader.PICC_IsNewCardPresent()) {
		String data = func.RemoveCard(zone);
		Serial.println(data);
		return;
	}

	//Scan Readers twice in case of collision
	for (int i = 0; i < 2; i++) {
		String cardSerialNumber = "";

		isCardPresent = zone.reader.PICC_ReadCardSerial();
		if (isCardPresent) {
			cardSerialNumber = GetCardSerialNumber(zone.reader);
			isMonster = func.CheckIfCardIsMonster(readBackBlock[0]);

			zone.UpdateCurrentMonster(cardSerialNumber);
		}

		if (isMonster) {
			func.CheckBattleAndFaceModes(zone);
		}

		//Collision Test -> If only one card is in proximity the second loop will always return null
		if (i == 1 && cardSerialNumber == "") return;

		eventType = func.SetEventType(zone, &isMonster, cardSerialNumber);

		String data = func.FormatEventInfo(zone, eventType, isMonster);
		Serial.println(data);

		zone.reader.PICC_HaltA();
		zone.reader.PCD_StopCrypto1();
	}
}

String GetCardSerialNumber(MFRC522 reader)
{
	read.ReadBlock(reader, readBackBlock);
	return (String((char*)readBackBlock)).substring(1, 16);
}