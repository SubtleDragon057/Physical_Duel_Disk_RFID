/*
  Name:    DuelDisk_ProjectATEM_Demo.ino
  Created: 1/22/2021 11:07:28 AM
  Author:  SubtleDragon057
*/

const bool handlerDebug = true;

#include <Wire.h>
#include <PN532.h>
#include <PN532_I2C.h>

#include "src\ZoneHandler.h"
#include "src\EventHandler.h"
#include "src\CommunicationsHandler.h"
#include "src\Core\Entities\Enums.h"

EventHandler eventHandler(handlerDebug);
CommunicationsHandler communicationsHandler(handlerDebug);
ZoneHandler zoneHandler(handlerDebug);

const byte numZones = 3;

PN532_I2C pnI2C(Wire);
PN532 reader(pnI2C);

byte attackSensorAddresses[numZones] = { 13, 14, 15 };
byte defenceSensorAddresses[numZones] = { 7, 8, 9 };
byte spellSensorAddresses[numZones] = { 10, 11, 12 };

void setup() {

	Serial.begin(115200);
	Wire.begin();

	zoneHandler.Initialize(numZones, attackSensorAddresses, reader,
		defenceSensorAddresses, spellSensorAddresses);

	Wire.begin(11);
	Wire.onReceive(HandleRecieve);
	Wire.onRequest(HandleRequest);

	Serial.println("Initialization Complete");
	Serial.println();
}

void loop() {

	// Cycle through each zone on the Duel Disk to check for any changes
	for (int i = 0; i < numZones; i++) {
		zoneHandler.CheckForTrippedSensor(i);

		for (byte j = 0; j < 3; j++) {			
			if (!zoneHandler.Zones[i].TrippedSensors[j]) continue;

			zoneHandler.Zones[i].TrippedSensors[j] = false;
			String eventData = eventHandler.GetFormattedEventData(zoneHandler.Zones[i], j);

			if (eventData == "") continue;
			communicationsHandler.HandleNewEvent(eventData);

			if (handlerDebug) {
				Serial.println();
				Serial.print("Event Info: ");
				Serial.println(eventData);
				Serial.println();
			}
		}
	}

	//Serial.println();
}

void HandleRecieve(int numBytes) {
	communicationsHandler.HandleRecieve();
}

void HandleRequest() {
	communicationsHandler.HandleRequest();
}