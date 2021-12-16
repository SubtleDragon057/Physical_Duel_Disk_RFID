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

		TrippedSensors[i] = true;
	}
}

void ZoneHandler::CheckRFIDReader(DualCardZone &zone, int sensorType) {
	
	bool hasNewCard = zone.ScanForNewCard();
	if (!hasNewCard) {
		HandleUpdateCard(zone, sensorType, true);		
		zone.StopScanning();
		return;
	}

	HandleUpdateCard(zone, sensorType);
	zone.StopScanning();
}

void ZoneHandler::HandleUpdateCard(DualCardZone& zone, int sensorType, bool isRemoval) {
	// TODO: This is currently broken
	if (isRemoval) {
		zone.UpdateCurrentMonster(zone.MonsterSerial, Enums::NoCard);
		return;
	}
	
	String cardSerialNumber = zone.GetCardSerialNumber();
	Enums::CardPosition position = Enums::NoCard;

	if (_debug) {
		Serial.print("Card Serial: "); Serial.println(cardSerialNumber);
	}

	// TODO: Handle Spell Placement/Removal
	if (sensorType == Enums::SpellTrap) {
		position = zone.ReadCurrentSpellPosition();
		zone.UpdateCurrentSpell(cardSerialNumber, position);
		return;
	}

	position = zone.ReadCurrentMonsterPosition();
	zone.UpdateCurrentMonster(cardSerialNumber, position);
}

void ZoneHandler::SelectMultiplexerAddress(uint8_t address) {
	if (address > 7) return;

	Wire.beginTransmission(_multiplexerAddress);
	Wire.write(1 << address);
	Wire.endTransmission();
}