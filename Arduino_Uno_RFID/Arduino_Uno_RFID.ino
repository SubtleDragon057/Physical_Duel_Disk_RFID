/*
  Name:    DuelDisk_ProjectATEM_Demo.ino
  Created: 1/22/2021 11:07:28 AM
  Author:  SubtleDragon057
*/

bool debug = false;
bool handlerDebug = true;

#include <MFRC522.h>
#include <SPI.h>
#include <Wire.h>

#include "src\ZoneHandler.h"
#include "src\EventHandler.h"
#include "src\CommunicationsHandler.h"
#include "src\Core\Entities\Components.h"
#include "src\Core\Entities\Enums.h"

ZoneHandler zoneHandler(handlerDebug);
EventHandler eventHandler(handlerDebug);
CommunicationsHandler communicationsHandler(handlerDebug);

const byte numZones = 3;

byte readerPins[numZones + 1] = { 
	2, 
	3, 
	4, 
	8 // Reset Pin
};
ProximitySensor attackSensors[numZones] = {
	ProximitySensor(5, "D1"),
	ProximitySensor(6, "D2"),
	ProximitySensor(7, "D3")
};
AnalogIR defenceSensors[numZones] = {
	AnalogIR(A0, "A1", debug),
	AnalogIR(A1, "A2", debug),
	AnalogIR(A2, "A3", debug)
};
AnalogIR spellSensors[numZones] = {
	AnalogIR(A3, "A4", debug),
	AnalogIR(A4, "A5", debug),
	AnalogIR(A5, "A6", debug)
};

void setup() {

	Serial.begin(9600); 
	SPI.begin();
	
	Wire.begin(11);
	Wire.onReceive(HandleRecieve);
	Wire.onRequest(HandleRequest);

	zoneHandler.Initialize(numZones, readerPins, attackSensors, 
		defenceSensors, spellSensors);

	Serial.println("Initialization Complete");
	Serial.println();
}

void loop() {
	
	// Cycle through each zone on the Duel Disk to check for any changes
	for (int i = 0; i < numZones; i++) {
		Enums::SensorType trippedSensor = zoneHandler.CheckForTrippedSensor(i);

		if (trippedSensor == Enums::None) continue;

		String eventData = eventHandler.GetFormattedEventData(zoneHandler.Zones[i], trippedSensor);
		
		if (eventData == "") return;
		Serial.println(eventData);
		communicationsHandler.HandleNewEvent(eventData);
	}
}

void HandleRecieve(int numBytes) {
	communicationsHandler.HandleRecieve();
}

void HandleRequest() {
	communicationsHandler.HandleRequest();
}