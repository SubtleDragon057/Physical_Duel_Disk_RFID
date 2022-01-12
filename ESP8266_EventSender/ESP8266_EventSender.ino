/* Adapted From:
   WebSocketClientSocketIO.ino
   Created on: 06.06.2016 by Links2004

   Adapted by: SubtleDragon057
   For use with: Project ATEM Duel Disk Proto
*/

#include <Wire.h>
#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

#include "src\ButtonHandler.h"
#include "src\SmartDuelEventHandler.h"
#include "src\CommunicationsHandler.h"
#include "src\StorageHandler.h"
#include "src\Secrets.h"

ButtonHandler buttonHandler;
CommunicationsHandler communicationsHandler;
SmartDuelEventHandler smartDuelEventHandler(communicationsHandler);
StorageHandler storageHandler(communicationsHandler);
SECRETS secrets;

const byte sdReaderPin = 5;

void setup() {    

  Serial.begin(115200);
  Wire.begin(SDA, SCL);

  communicationsHandler.Initialize(secrets.networkName, secrets.networkPass);
  
  bool connected = false;
  for (byte i = 10; i > 0; i--) {
	  connected = SD.begin(sdReaderPin);
	  if (connected) break;
  }
  storageHandler.Initialize(connected);
  buttonHandler.Initialize();
  smartDuelEventHandler.Connect(secrets.socketIP, secrets.socketPort);

  Serial.printf("\n");
}

void loop() {

	while (!smartDuelEventHandler.IsConnected()) {
		smartDuelEventHandler.ListenToServer();
	}

	while (!storageHandler.IsDeckSet) {
		smartDuelEventHandler.ListenToServer();
		buttonHandler.CheckButtons();
		storageHandler.ChooseDeck(buttonHandler.ButtonEvents);
	}

	while (!smartDuelEventHandler.IsInDuelRoom) {
		smartDuelEventHandler.ListenToServer();

		buttonHandler.CheckButtons();
		smartDuelEventHandler.HandleLobby(buttonHandler.ButtonEvents, storageHandler.DeckList);
	}

	// Set to false to allow user to choose new deck when entering lobby again
	storageHandler.IsDeckSet = false;

	while (smartDuelEventHandler.IsInDuelRoom && !smartDuelEventHandler.IsDueling) {
		smartDuelEventHandler.ListenToServer();
		buttonHandler.CheckButtons();
		smartDuelEventHandler.HandleDuelRoom(buttonHandler.ButtonEvents);
	}

	smartDuelEventHandler.ListenToServer();
	buttonHandler.CheckButtons();
	smartDuelEventHandler.HandleButtonInteraction(buttonHandler.ButtonEvents);

	while (!smartDuelEventHandler.HasAttackTarget()) {
		smartDuelEventHandler.ListenToServer();
		buttonHandler.CheckButtons();
		smartDuelEventHandler.HandleButtonInteraction(buttonHandler.ButtonEvents, true);
	}

	String output = communicationsHandler.PollForNewEvent();
	if (output == "No Events!!") return;

	smartDuelEventHandler.HandleOutgoingEvent(output);
}