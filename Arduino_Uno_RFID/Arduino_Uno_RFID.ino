/*
  Name:    DuelDisk_ProjectATEM_Demo.ino
  Created: 1/22/2021 11:07:28 AM
  Author:  SubtleDragon057
*/

bool handlerDebug = true;

#include <SPI.h>
#include <Wire.h>
#include <PN532.h>
#include <PN532_I2C.h>

#include "src\ZoneHandler.h"
#include "src\EventHandler.h"
#include "src\CommunicationsHandler.h"
#include "src\Core\Entities\Enums.h"

ZoneHandler zoneHandler(handlerDebug);
EventHandler eventHandler(handlerDebug);
CommunicationsHandler communicationsHandler(handlerDebug);

const byte numZones = 3;

PN532_I2C pnI2C(Wire);
PN532 reader(pnI2C);

byte readerPins[numZones + 1] = { 2, 3, 4, 8 }; // Last pin is Reset
byte attackSensorPins[numZones] = {	5, 6, 7 };
byte defenceSensorPins[numZones] = { A0, A1, A2 };
byte spellSensorPins[numZones] = { A4, A5, A6 };

void setup() {

	Serial.begin(9600); 
	SPI.begin();

	reader.begin();
	reader.SAMConfig();
	reader.setPassiveActivationRetries(5);

	Wire.begin(11);
	Wire.onReceive(HandleRecieve);
	Wire.onRequest(HandleRequest);

	zoneHandler.Initialize(numZones, readerPins, attackSensorPins, reader,
		defenceSensorPins, spellSensorPins);

	Serial.println("Initialization Complete");
	Serial.println();
}

void loop() {

	// Cycle through each zone on the Duel Disk to check for any changes
	for (int i = 0; i < numZones; i++) {
		Enums::SensorType trippedSensor = zoneHandler.CheckForTrippedSensor(2);

		if (trippedSensor == Enums::None) continue;

		String eventData = eventHandler.GetFormattedEventData(zoneHandler.Zones[2], trippedSensor);
		
		if (eventData == "") return;
		communicationsHandler.HandleNewEvent(eventData);
		
		Serial.println();
		Serial.print("Event Info: ");
		Serial.println(eventData);
		Serial.println();
	}
}

void HandleRecieve(int numBytes) {
	communicationsHandler.HandleRecieve();
}

void HandleRequest() {
	communicationsHandler.HandleRequest();
}