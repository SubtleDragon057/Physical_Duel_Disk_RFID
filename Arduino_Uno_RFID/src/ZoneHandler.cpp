#include "ZoneHandler.h"
#include "Wire.h"

ZoneHandler::ZoneHandler(bool debug)
{
	_debug = debug;
}

void ZoneHandler::Initialize(byte numZones, byte attackSensorAddresses[], PN532 &reader,
	byte defenceSensorAddresses[], byte spellSensorAddresses[]) {

	byte error;
	do {
		Wire.beginTransmission(_multiplexerAddress);
		error = Wire.endTransmission();

		if (_debug) {
			if (error == 0) {
				Serial.print("I2C device found at address 0x");
			}
			else if (error == 4) {
				Serial.print("Unknow error at address 0x");
			}
			Serial.println(_multiplexerAddress, HEX);
		}

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

		/*if (_debug) {
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
		}*/

		// TODO: This always fails, but doesn't seem necessary
		/*uint8_t success = reader.SAMConfig();
		if (!success) {
			Serial.print("[DEBUG] SAMConfig failed on Zone ");
			Serial.println(i);
		}*/

		reader.setPassiveActivationRetries(5);

		Zones[i].Initialize(i, reader,
			_muxChannels[attackSensorAddresses[i]],
			_muxChannels[defenceSensorAddresses[i]],
			_muxChannels[spellSensorAddresses[i]]
		);
	}

	delay(10);
}

void ZoneHandler::CheckForTrippedSensor(int zoneNumber) {
	
	if (_debug) {
		Serial.print("Checking Zone: "); Serial.println(zoneNumber);
	}
	
	Zones[zoneNumber].CheckForTrippedSensors();
	for (byte i = 0; i < 3; i++) {
		if (!Zones[zoneNumber].TrippedSensors[i]) continue;
		
		delay(100); // delay to ensure card is fully placed

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
		zone.StopScanning();
		return;
	}

	HandleUpdateCard(zone, sensorType, position);
	zone.StopScanning();
}

void ZoneHandler::HandleUpdateCard(DualCardZone& zone, int sensorType, Enums::CardPosition position) {
	
	String cardSerialNumber = zone.GetCardSerialNumber();

	if (_debug) {
		Serial.print("Card Serial: "); Serial.println(cardSerialNumber);
	}

	if (cardSerialNumber == zone.MonsterSerial || cardSerialNumber == zone.SpellSerial) {
		cardSerialNumber = zone.GetCardSerialNumber();
	}

	if (sensorType == Enums::SpellTrap) {
		zone.UpdateCurrentSpell(cardSerialNumber, position);
		return;
	}

	zone.UpdateCurrentMonster(cardSerialNumber, position);
}

void ZoneHandler::HandleRemoveCard(DualCardZone& zone, int sensorType) {

	if (sensorType == Enums::SpellTrap) {
		zone.UpdateCurrentSpell(zone.SpellSerial, Enums::NoCard);
		return;
	}

	zone.UpdateCurrentMonster(zone.MonsterSerial, Enums::NoCard);
}

void ZoneHandler::SelectMultiplexerAddress(uint8_t address) {
	if (address > 7) return;

	Wire.beginTransmission(_multiplexerAddress);
	Wire.write(1 << address);
	Wire.endTransmission();
}