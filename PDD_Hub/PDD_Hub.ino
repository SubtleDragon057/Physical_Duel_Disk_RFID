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
#include "src\PeripheralsHandler.h"
#include "src\StorageHandler.h"

ButtonHandler buttonHandler;
PeripheralsHandler peripheralsHandler;
StorageHandler storageHandler(peripheralsHandler);
SmartDuelEventHandler smartDuelEventHandler(peripheralsHandler, storageHandler);

const byte sdReaderPin = 5;
const byte intPin = 16;
volatile bool hasNewEvent = false;
portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
void IRAM_ATTR handleIncomingEvent() {
	Serial.printf("Interrupt Detected!\n");
	
	if (!peripheralsHandler.IsBladeConnected) {
		portENTER_CRITICAL_ISR(&mux);
		peripheralsHandler.IsBladeConnected = true;
		portEXIT_CRITICAL_ISR(&mux);

		return;
	}

	switch (smartDuelEventHandler.DuelRoomState) {
		case Enums::DuelRoomState::IsInDuel:
		case Enums::DuelRoomState::WaitingForAttackTarget:
			portENTER_CRITICAL_ISR(&mux);
			hasNewEvent = true;
			portEXIT_CRITICAL_ISR(&mux);
			break;
		default:
			Serial.printf("No State Match: %i\n", smartDuelEventHandler.DuelRoomState);
			break;
	}
}

void setup() {    

  Serial.begin(115200);
  Wire.begin(SDA, SCL);

  attachInterrupt(digitalPinToInterrupt(intPin), handleIncomingEvent, FALLING);
  peripheralsHandler.InitializeCommunications();
  
  bool sdReaderCnnected = false;
  for (byte i = 10; i > 0; i--) {
	  sdReaderCnnected = SD.begin(sdReaderPin);
	  if (sdReaderCnnected) break;
  }
  storageHandler.Initialize(sdReaderCnnected);
  buttonHandler.Initialize();
  smartDuelEventHandler.Connect();

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

	if (!hasNewEvent) return;
	portENTER_CRITICAL(&mux);
	hasNewEvent = false;
	portEXIT_CRITICAL(&mux);

	String output = peripheralsHandler.GetNewEventData();
	Serial.printf("Event Info: %s\n", output.c_str());
	if (output == "" || output == "Failure") {
		Serial.printf("Event was empty!\n");
		return;
	}
	smartDuelEventHandler.HandleOutgoingEvent(output);
}