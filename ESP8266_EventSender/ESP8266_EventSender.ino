/* Adapted From:
   WebSocketClientSocketIO.ino
   Created on: 06.06.2016 by Links2004

   Adapted by: SubtleDragon057
   For use with: Project ATEM Duel Disk Proto
*/

#define DEBUG

#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include <Wire.h>

#include "src\ButtonHandler.h"
#include "src\SmartDuelEventHandler.h"
#include "src\CommunicationsHandler.h"
#include "src\StorageHandler.h"
#include "src\Secrets.h"
#include "src\Core\Entities\Button.h"

ButtonHandler buttonHandler;
CommunicationsHandler communicationsHandler;
SmartDuelEventHandler smartDuelEventHandler(communicationsHandler);
StorageHandler storageHandler(communicationsHandler);
SECRETS secrets;

#ifdef ESP8266;
byte button1Pin = 0;
byte button2Pin = 2;
byte button3Pin = 14;
byte button4Pin = 12;
byte button5Pin = 13;
#endif
#ifdef ESP32
const byte button1Pin = 32;
const byte button2Pin = 33;
const byte button3Pin = 25;
const byte button4Pin = 26;
const byte button5Pin = 27;
#endif

const byte sdReaderPin = 5;
Button buttons[5] = {
    Button("Button1", button1Pin),
    Button("Button2", button2Pin),
    Button("Button3", button3Pin),
    Button("Button4", button4Pin),
    Button("Button5", button5Pin)
};

int deckList[35] = {
  25652259,
  11549357,
  90876561,
  62651957,
  84636823,
  65622692,
  98502113,
  5818798,
  11321183,
  14898066,
  71413901,
  64500000,
  99785935,
  46986414,
  76909279,
  4796100,
  78193831,
  89631139,
  46363422,
  31553716,
  39256679,
  91998119,
  75347539,
  77207191
};

void setup() {    

  Serial.begin(115200);
  Wire.begin(SDA, SCL);
  while (!SD.begin(sdReaderPin)) {
	  HandleRetry("[ERROR] SD Reader Failed To Connect");
  }

  communicationsHandler.Initialize(secrets.networkName, secrets.networkPass);
  buttonHandler.Initialize(buttons);
  smartDuelEventHandler.Connect(secrets.socketIP, secrets.socketPort);
  Serial.println();
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

void HandleRetry(String errorMessage) {
	static int i = 0;
	delay(100);
	i++;

	if (i >= 10) {
		Serial.println(errorMessage);
		i = 0;
	}
}