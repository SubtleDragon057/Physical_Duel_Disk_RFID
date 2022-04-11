/* 
   Name: PDD_Hub.ino
   Updated: 2/14/2022 10:51:32 AM
   Author:  SubtleDragon057
*/

#define DEBUG_Main

#include <Wire.h>
#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include <PN532.h>
#include <PN532_I2C.h>

#include "src\ButtonHandler.h"
#include "src\SmartDuelEventHandler.h"
#include "src\PeripheralsHandler.h"
#include "src\StorageHandler.h"
#include "src\ZoneHandler.h"

PeripheralsHandler peripheralsHandler;
ButtonHandler buttonHandler(peripheralsHandler);
StorageHandler storageHandler(peripheralsHandler);
SmartDuelEventHandler smartDuelEventHandler(peripheralsHandler, storageHandler);
ZoneHandler zoneHandler(smartDuelEventHandler, peripheralsHandler);

PN532_I2C pnI2C(Wire);
PN532 reader(pnI2C);

const byte numZones = 3;
const byte sdReaderPin = 5;

void setup() {    

  Serial.begin(115200);
  Wire.begin(SDA, SCL);

  peripheralsHandler.InitializeCommunications();
  
  bool sdReaderCnnected = false;
  for (byte i = 10; i > 0; i--) {
	  sdReaderCnnected = SD.begin(sdReaderPin);
	  if (sdReaderCnnected) break;
  }
  storageHandler.Initialize(sdReaderCnnected);
  buttonHandler.Initialize();
  smartDuelEventHandler.Connect();
  zoneHandler.Initialize(numZones, reader);

  Serial.printf("\n");
}

void loop() {

	// Allow player to set deck while waiting for Server
	while (!smartDuelEventHandler.IsConnected() || !storageHandler.IsDeckSet) {
		smartDuelEventHandler.ListenToServer();
		buttonHandler.CheckButtons();
		if (!storageHandler.IsDeckSet) {
			storageHandler.ChooseDeck(buttonHandler.ButtonEvents);
		}
	}

	smartDuelEventHandler.ListenToServer();
	buttonHandler.CheckButtons();
	smartDuelEventHandler.HandleDuelRoomState(buttonHandler.ButtonEvents);

	if (smartDuelEventHandler.DuelRoomState == Enums::Lobby ||
		smartDuelEventHandler.DuelRoomState == Enums::WaitingForOpponent) return;

	// Cycle through each zone on the Duel Disk to check for any changes
	for (int i = 0; i < numZones; i++) {
		zoneHandler.CheckForNewEvents(i);
	}
}