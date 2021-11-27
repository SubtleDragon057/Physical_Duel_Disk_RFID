/*
  Name:    DuelDisk_ProjectATEM_Demo.ino
  Created: 1/22/2021 11:07:28 AM
  Author:  SubtleDragon057
*/

bool debug = false;

#include <MFRC522.h>
#include <SPI.h>
#include <Wire.h>

#include "src\ZoneHandler.h"
#include "src\EventHandler.h"
#include "src\Core\Entities\Components.h"
#include "src\Core\Entities\Enums.h"

ZoneHandler zoneHandler;
EventHandler eventHandler;

const byte numZones = 3;

String message = "";

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
	Wire.onReceive(ConnectToESP);
	Wire.onRequest(SendToESP);

	zoneHandler.Initialize(numZones, readerPins, attackSensors, 
		defenceSensors, spellSensors);

	Serial.println("Initialization Complete");
	Serial.println();
}


void loop() {

	delay(100);	

}

void ConnectToESP(int numBytes) {
	message = "";

	while (Wire.available()) {
		char incoming = Wire.read();
		message += incoming;
	}
}

void SendToESP() {
	if (message == "SubtleDragon") {
		Wire.write("057");
		return;
	}
	
	// Cycle through each zone on the Duel Disk to check for any changes
	for (int i = 0; i < numZones; i++) {
		Enums::SensorType sensor = zoneHandler.CheckZone(i);

		if (sensor == Enums::None) continue;

		Enums::Events eventType = eventHandler.SetEventType(zoneHandler.Zones[i], sensor);
		String data = eventHandler.FormatEventInfo(zoneHandler.Zones[i], eventType, sensor);

		char* output;
		data.toCharArray(output, 11);

		Serial.println(data);
		Wire.write(output);
		return; // TODO: This may cause multiple cards placed in the same frame to be missed!
	}

	Wire.write("12345678901");
}