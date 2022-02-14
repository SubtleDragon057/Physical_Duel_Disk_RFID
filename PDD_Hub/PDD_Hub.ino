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
const byte intPin = 16;
volatile bool hasNewEvent = false;
portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
void IRAM_ATTR handleIncomingEvent() {
	if (!smartDuelEventHandler.IsDueling) return;

	portENTER_CRITICAL_ISR(&mux);
	hasNewEvent = true;
	portEXIT_CRITICAL_ISR(&mux);

#ifdef DEBUG_Main
	Serial.printf("Incoming Event!\n");
#endif // DEBUG_MAIN
}

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

  attachInterrupt(digitalPinToInterrupt(intPin), handleIncomingEvent, FALLING);

  Serial.printf("\n");
}

void loop() {

	while (!smartDuelEventHandler.IsConnected() || !storageHandler.IsDeckSet) {
		smartDuelEventHandler.ListenToServer();
		buttonHandler.CheckButtons();
		if (!storageHandler.IsDeckSet) {
			storageHandler.ChooseDeck(buttonHandler.ButtonEvents);
		}
	}

	while (!smartDuelEventHandler.IsInDuelRoom) {
		smartDuelEventHandler.ListenToServer();
		buttonHandler.CheckButtons();
		smartDuelEventHandler.HandleLobby(buttonHandler.ButtonEvents, storageHandler.DeckList);
	}

	// TODO: Find better implementation - this one is broken
	// Set to false to allow user to choose new deck when entering lobby again
	//storageHandler.IsDeckSet = false;

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

	if (!hasNewEvent) return;
	portENTER_CRITICAL(&mux);
	hasNewEvent = false;
	portEXIT_CRITICAL(&mux);

	String output = communicationsHandler.GetNewEventData();
	Serial.printf("Event Info: %s\n", output.c_str());
	if (output == "None" || output == "") {
		Serial.printf("Event was empty!\n");
		return;
	}
	smartDuelEventHandler.HandleOutgoingEvent(output);
}