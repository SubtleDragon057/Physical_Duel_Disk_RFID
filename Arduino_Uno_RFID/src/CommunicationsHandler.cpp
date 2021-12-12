#include "CommunicationsHandler.h"
#include "Arduino.h"
#include "Wire.h"
#include "Core\Entities\Enums.h"

CommunicationsHandler::CommunicationsHandler(bool debug)
{
	_debug = true;
}

void CommunicationsHandler::InitializeNFCReaders(byte numZones, PN532 &reader) {
	Serial.println("[BOOT] Initialize NFC Readers");
	
	byte error;
	do {
		Wire.beginTransmission(multiplexerAddress);
		error = Wire.endTransmission();

		if (_debug) {
			if (error == 0) {
				Serial.print("I2C device found at address 0x0");
			}
			else if (error == 4) {
				Serial.print("Unknow error at address 0x0");
			}
			Serial.println(multiplexerAddress, HEX);
		}

		delay(50);
	} while (error != 0);

	Serial.println("[BOOT] Multiplexer Found");
	
	for (byte i = 0; i < numZones; i++) {
		SelectMultiplexerAddress(i);
		reader.begin();

		if (_debug) {
			Serial.print(F("[DEBUG] "));
			uint32_t versiondata = reader.getFirmwareVersion();
			if (!versiondata) {
				Serial.print("Didn't find PN53x board on Zone ");
				Serial.println(i);
				continue;
			}
			// Got ok data, print it out!
			Serial.print("Found chip PN5"); Serial.println((versiondata >> 24) & 0xFF, HEX);
			Serial.print("Firmware ver. "); Serial.print((versiondata >> 16) & 0xFF, DEC);
			Serial.print('.'); Serial.println((versiondata >> 8) & 0xFF, DEC);
		}

		uint8_t success = reader.SAMConfig();
		if (!success) {
			Serial.print("[DEBUG] SAMConfig failed on Zone ");
			Serial.println(i);
		}

		reader.setPassiveActivationRetries(5);
	}
}

void CommunicationsHandler::HandleRecieve() {
	while (Wire.available()) {
		_recievedData = Wire.read();
	}
}

void CommunicationsHandler::HandleRequest() {
	if (_recievedData == Enums::Communication::Connection) {
		Wire.write("057");
	}
	else if (_recievedData == Enums::Communication::HasNewEvent) {
		byte hasChange = _hasNewEvent ? 0x01 : 0x00;
		Wire.write(hasChange);
	}
	else if (_recievedData == Enums::Communication::GetEventInfo) {
		Wire.write(_newEventData);
		_hasNewEvent = false;
	}
}

void CommunicationsHandler::HandleNewEvent(String eventData) {
	_hasNewEvent = true;
	eventData.toCharArray(_newEventData, 12);
}

void CommunicationsHandler::SelectMultiplexerAddress(uint8_t address) {
	if (address > 7) return;

	Wire.beginTransmission(multiplexerAddress);
	Wire.write(1 << address);
	Wire.endTransmission();
}