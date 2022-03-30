/*
  Name:    PDD_Blade.ino
  Created: 1/22/2021 11:07:28 AM
  Updated: 2/14/2022 10:51:32 AM
  Author:  SubtleDragon057
*/

#include <Wire.h>
#include <PN532.h>
#include <PN532_I2C.h>

#include "src\ZoneHandler.h"
#include "src\EventHandler.h"
#include "src\CommunicationsHandler.h"

EventHandler eventHandler;
CommunicationsHandler communicationsHandler;
ZoneHandler zoneHandler(eventHandler);

const byte numZones = 3;

PN532_I2C pnI2C(Wire);
PN532 reader(pnI2C);

void setup() {

	Serial.begin(115200);
	Wire.begin();

	zoneHandler.Initialize(numZones, reader);

	Wire.begin(11);
	Wire.onReceive(HandleRecieve);
	Wire.onRequest(HandleRequest);

	Serial.println(F("Initialization Complete\n"));
}

void loop() {

	if (communicationsHandler.EnableWriteMode) {
		bool success = zoneHandler.EnableWriteMode(communicationsHandler.IncomingCardID);
		if (success) {
			communicationsHandler.IncomingCardID = "";
			communicationsHandler.GetNextCard();
		}
	}

	if (!communicationsHandler.IsInDuel) return;
	
	// Cycle through each zone on the Duel Disk to check for any changes
	for (int i = 0; i < numZones; i++) {
		uint8_t hasNewEvent = zoneHandler.CheckForNewEvents(i);		
		if (!hasNewEvent) continue;

		communicationsHandler.HandleNewEvent(hasNewEvent, zoneHandler.Events[i]);
	}

	// Allow for incoming I2C messages
	Wire.begin(11);
}

void HandleRecieve(int numBytes) {
	communicationsHandler.HandleRecieve();
}

void HandleRequest() {
	communicationsHandler.HandleRequest();
}