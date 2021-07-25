/*
  Name:    DuelDisk_ProjectATEM_Demo.ino
  Created: 1/22/2021 11:07:28 AM
  Author:  SubtleDragon057
*/

//#define ANTENNA DEBUG

#include <SPI.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>

#include "LocalFunctions.h"

//---------------------------------------------------------------------------------//
//----------------------------------Globals----------------------------------------//
//---------------------------------------------------------------------------------//

SoftwareSerial toESP(0, 1);
LocalFunctions func;

//RFID Readers
const byte numReaders = 3;
const byte ssPins[] = { 2, 3, 4 };
const byte resetPin = 8;
int currentReader;
MFRC522 cardSlots[numReaders];
MFRC522::MIFARE_Key key;

//Monsters
String currentMonsterIDs[numReaders];

//Spell/Trap
String currentSpells[numReaders];
byte spellButton1 = 5;
byte spellButton2 = 6;
byte spellButton3 = 7;

//Attack or Defence
int cardBattleMode[numReaders];
int lightValueBattle[numReaders];
int currentLightValueBattle[numReaders];
byte lightPinBattle[numReaders] = { A0, A1, A2 };

//Face Up/Face Down
int cardFaceMode[numReaders];
int lightValueFace[numReaders];
int currentLightValueFace[numReaders];
byte lightPinFace[numReaders] = { A3, A4, A5 };

//Info for Writing & Reading the blocks
byte block = 4;
byte readBackBlock[16];

//This is where you put the cards to write to the NFC chips. See NOTE for details
const byte cardsToRead = 1;
int cardNames[cardsToRead][9] = {
  //{2, 5, 5, 7, 6, 1, 7, 9, 2}, //Black Luster Ritual
  //{1, 0, 5, 4, 0, 5, 6, 9, 4}, //Black Luster Soldier
  //{1, 4, 6, 9, 8, 6, 4, 1, 4}, //Dark Magician
  //{1, 1, 0, 0, 0, 0, 0, 2, 0}, //Slifer the Sky Dragon
  //{1, 7, 8, 1, 9, 3, 8, 3, 1}, //Buster Blader
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

//-------------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------Enums-------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------------//

enum Events {
  NoEvent,
  Summon,
  Remove,
  PositionChange,
  SetSpellTrap,
  ActivateSpellTrap,
};

void setup() {

  Serial.begin(9600);
  SPI.begin();
  toESP.begin(9600);

  for (int i = 0; i < numReaders; i++) {
    cardSlots[i].PCD_Init(ssPins[i], resetPin);
    cardSlots[i].PCD_SetAntennaGain(MFRC522::PCD_RxGain::RxGain_avg);

#ifdef ANTENNA DEBUG
    func.DebugRFIDConnection(i, ssPins, cardSlots);
#endif
    delay(10);
  }

  //set up Spell/Trap activation buttons
  pinMode(spellButton1, INPUT_PULLUP);
  pinMode(spellButton2, INPUT_PULLUP);
  pinMode(spellButton3, INPUT_PULLUP);

  //set up key to read the NFC chips
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  func.InitializeWriteFeatures(key, block, cardsToRead);
}


void loop() {

  //This starts the "Write Mode" feature on the Duel Disk
  if (digitalRead(spellButton1) == LOW && digitalRead(spellButton3) == LOW) {
    delay(5000);
    if (digitalRead(spellButton1) == LOW && digitalRead(spellButton3) == LOW) {
      func.WriteModeActivated(cardSlots[1], cardNames);
    }
    else {
      Serial.println("buttons released too early");
    }
  }


  //Cycle through each card slot on the Duel Disk to check for any changes
  for (int slot = 0; slot < numReaders; slot++) {

    bool cardNoLongerPresent;
    bool isMonster = false;
    bool changedValue = false;
    bool newBattleMode = false;
    bool newFaceMode = false;
    Events eventType = NoEvent;


    cardSlots[slot].PCD_Init();

    byte bufferATQA[2];
    byte bufferSize = sizeof(bufferATQA);

    // Reset baud rates
    cardSlots[slot].PCD_WriteRegister(cardSlots[slot].TxModeReg, 0x00);
    cardSlots[slot].PCD_WriteRegister(cardSlots[slot].RxModeReg, 0x00);
    // Reset ModWidthReg
    cardSlots[slot].PCD_WriteRegister(cardSlots[slot].ModWidthReg, 0x26);


    MFRC522::StatusCode result = cardSlots[slot].PICC_RequestA(bufferATQA, &bufferSize);
    
    if (result == MFRC522::STATUS_COLLISION || result == MFRC522::STATUS_OK) {

      Serial.println(result);
      for (int i = 0; i < 2; i++) {

        currentReader = slot;
        int spellSlot;
        String readRFID = "";
        int battleMode;
        int faceMode;

        //Check to see if a new Yu-Gi-Oh card has been placed on the Duel Disk
        if (cardSlots[slot].PICC_IsNewCardPresent() && cardSlots[slot].PICC_ReadCardSerial()) {

          cardNoLongerPresent = false;

          func.ReadBlock(cardSlots[slot], readBackBlock);
          readRFID = (String((char*)readBackBlock)).substring(1, 16);

          //Check for Spell or Trap Card
          if (readBackBlock[0] != 50 && readBackBlock[0] != 51) {
            isMonster = true;
          }
          else if (readBackBlock[0] == 50 || readBackBlock[0] == 51) {
            spellSlot = (slot + 3);
          }
        } //Check for card
        else {
          cardNoLongerPresent = true;
        }

        if (isMonster) {

          //Check for Attack or Defence mode
          newBattleMode = func.CheckMode(lightValueBattle,
                                    lightPinBattle,
                                    slot,
                                    currentLightValueBattle,
                                    cardBattleMode);

          //Check if card is Face Up, or Face Down
          newFaceMode = func.CheckMode(lightValueFace,
                                  lightPinFace,
                                  slot,
                                  currentLightValueFace,
                                  cardFaceMode);
        }

        //Collision Test -> If only one card is in proximity the second loop will always return null
        if (i == 1 && readRFID == "") {
          changedValue = false;
        }
        //If a change has been registered on this Card Slot the info is passed along
        else {
          //Changes to Monster Zone
          if (isMonster && readRFID != currentMonsterIDs[slot]) {
            changedValue = true;
            eventType = Summon;
            currentMonsterIDs[slot] = readRFID;
            if (currentMonsterIDs[slot] == "") {
              eventType = Remove;
            }
          }
          else if (isMonster && (newBattleMode || newFaceMode)) {
            changedValue = true;
            eventType = PositionChange;
            currentLightValueBattle[slot] = lightValueBattle[slot];
            currentLightValueFace[slot] = lightValueFace[slot];
          }
          //Changes to Spell/Trap Zone
          else if (!isMonster && readRFID != currentSpells[slot]) {
            changedValue = true;
            eventType = SetSpellTrap;
            currentSpells[slot] = readRFID;
            if (currentSpells[slot] == "") {
              eventType = Remove;
            }
          }
          //Override constant spell bool to allow monster to disappear when card not present
          else if (cardNoLongerPresent && readRFID != currentMonsterIDs[slot]) {
            changedValue = true;
            eventType = Remove;
            isMonster = true;
            currentMonsterIDs[slot] = readRFID;
          }
        } //check for changed info

        //Here is where the information gets sent from the Duel Disk to the SSD and SDG
        if (changedValue) {
          Serial.print(eventType);
          Serial.print(',');

          //Card Zone
          if (isMonster) {
            Serial.print(slot);
          }
          else if (!isMonster) {
            Serial.print(spellSlot);
          }
          Serial.print(',');

          //Card ID and Orientation
          if (isMonster) {
            if (currentMonsterIDs[slot] == "") {
              Serial.print("00000000");
            }
            else {
              Serial.print(currentMonsterIDs[slot]);
            }
            Serial.print(',');
            Serial.print(cardBattleMode[slot]);
            Serial.print(',');
            Serial.print(faceMode);
          }
          else if (!isMonster) {
            if (currentSpells[slot] == "") {
              Serial.print("0-0");
            }
            else {
              Serial.print(currentSpells[slot]);
            }
            Serial.print(',');
            Serial.print('0'); //Spell Cards default to Face Down. Buttons will be used to 'Activate' them...eventually
          }
          Serial.println();
        } //Changed Value

        cardSlots[slot].PICC_HaltA();
        cardSlots[slot].PCD_StopCrypto1();
      }//for Loop
    } //Zone Loop
  } //Reader Loop

  delay(500);

#ifdef DEBUG
  Serial.println(F("Restarting Loop"));
#endif
} //Loop
