#include "ZoneHandler.h"
#include "Wire.h"

//#define DEBUG_ZH
//#define DEBUG_Readers

ZoneHandler::ZoneHandler()
{
}

void ZoneHandler::Initialize(byte numZones, PN532& reader, const byte attackSensorAddresses[],
	const byte defenceSensorAddresses[], const byte spellSensorAddresses[]) {

	byte error;
	do {
		Wire.beginTransmission(_multiplexerAddress);
		error = Wire.endTransmission();
		delay(50);
	} while (error != 0);

	pinMode(2, OUTPUT);
	pinMode(3, OUTPUT);
	pinMode(4, OUTPUT);
	pinMode(5, OUTPUT);
	pinMode(A0, INPUT);

	for (byte i = 0; i < numZones; i++) {
		SelectMultiplexerAddress(i);
		reader.begin();
		reader.performRFTest();

#ifdef DEBUG_Readers
		Serial.print(F("[DEBUG] "));
		uint32_t versiondata = reader.getFirmwareVersion();
		if (!versiondata) {
			Serial.print("Didn't find PN53x board on Zone ");
			Serial.println(i);
			continue;
		}
		Serial.print("Found chip PN5"); Serial.println((versiondata >> 24) & 0xFF, HEX);
		Serial.print("Firmware ver. "); Serial.print((versiondata >> 16) & 0xFF, DEC);
		Serial.print('.'); Serial.println((versiondata >> 8) & 0xFF, DEC);
#endif // DEBUG_Readers

		reader.setPassiveActivationRetries(10);

		Zones[i].Initialize(i, reader,
			_muxChannels[attackSensorAddresses[i]],
			_muxChannels[defenceSensorAddresses[i]],
			_muxChannels[spellSensorAddresses[i]]
		);
	}

	delay(10);
}

void ZoneHandler::CheckForTrippedSensor(int zoneNumber) {
	
#ifdef DEBUG_ZH
	//Serial.print("Checking Zone: "); Serial.println(zoneNumber);
#endif
	
	Zones[zoneNumber].CheckForTrippedSensors();
	for (byte i = 0; i < 3; i++) {
		if (!Zones[zoneNumber].TrippedSensors[i]) continue;
		
		delay(250); // delay to ensure card is fully placed

		SelectMultiplexerAddress(zoneNumber);
		CheckRFIDReader(Zones[zoneNumber], i);
	}
}

void ZoneHandler::CheckRFIDReader(DualCardZone &zone, int sensorType) {
	
	bool hasNewCard = zone.ScanForNewCard();
	Enums::CardPosition position = Enums::NoCard;
	if (sensorType == Enums::SpellTrap) {
		position = zone.ReadCurrentSpellPosition();
	}
	else {
		position = zone.ReadCurrentMonsterPosition();
	}

	if (!hasNewCard || position == Enums::NoCard) {
		HandleRemoveCard(zone, sensorType);
		//zone.StopScanning();
		return;
	}

	HandleUpdateCard(zone, sensorType, position);
	//zone.StopScanning();
}

void ZoneHandler::HandleUpdateCard(DualCardZone& zone, int sensorType, Enums::CardPosition position) {
	
	String cardSerialNumber = zone.GetCardSerialNumber();

#ifdef DEBUG_ZH
	Serial.println("HandleUpdateCard()");
	Serial.print("Card Serial: "); Serial.println(cardSerialNumber);
#endif // DEBUG

	if (cardSerialNumber == "Failure") return;
	
	if (sensorType == Enums::SpellTrap) {
		if (cardSerialNumber == zone.MonsterSerial) {
			delay(100);
			cardSerialNumber = zone.GetCardSerialNumber();

			if (cardSerialNumber == zone.MonsterSerial) return;
		}
		zone.UpdateCurrentSpell(cardSerialNumber, position);
		return;
	}

	if (cardSerialNumber == zone.SpellSerial) {
		delay(100);
		cardSerialNumber = zone.GetCardSerialNumber();

		if (cardSerialNumber == zone.SpellSerial) return;
	}
	zone.UpdateCurrentMonster(cardSerialNumber, position);
}

void ZoneHandler::HandleRemoveCard(DualCardZone& zone, int sensorType) {

#ifdef DEBUG_ZH
	Serial.println("HandleRemoveCard()");
#endif // DEBUG_ZH
	
	if (sensorType == Enums::SpellTrap) {
		zone.UpdateCurrentSpell(zone.SpellSerial, Enums::NoCard);
		return;
	}

	zone.UpdateCurrentMonster(zone.MonsterSerial, Enums::NoCard);
}

bool ZoneHandler::EnableWriteMode(String cardID) {
	if (cardID == "") return false;
	byte useZone = 0;
	
	SelectMultiplexerAddress(useZone);
	bool cardAvailable = Zones[useZone].ScanForNewCard();
	if (!cardAvailable) {
		Serial.println("Waiting For Card");
		delay(2000);
		return false;
	}

	Serial.println("Card Found!");
	delay(2000); // Ensure Card is fully placed

	bool success = Zones[useZone].WriteRFIDTag(cardID);
	
	Serial.print("Written ID: ");
	if (success) Serial.println(Zones[useZone].GetCardSerialNumber());
	else {
		Serial.println("Write Failed");
		return false;
	}

	while(Zones[useZone].ScanForNewCard()) {
		Serial.println("Please Remove Card");
		delay(2000);
	}

	return true;
}

void ZoneHandler::SelectMultiplexerAddress(uint8_t address) {
	if (address > 7) return;

	Wire.beginTransmission(_multiplexerAddress);
	Wire.write(1 << address);
	Wire.endTransmission();
}