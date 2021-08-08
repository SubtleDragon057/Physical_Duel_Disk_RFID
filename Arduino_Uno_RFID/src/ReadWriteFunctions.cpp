#include "ReadWriteFunctions.h"

Read::Read()
{
}

void Read::Initialize(byte block)
{
	_block = block;

	for (byte i = 0; i < 6; i++) {
		_key.keyByte[i] = 0xFF;
	}
}
void Read::Initialize(byte block, byte customKey[])
{
	_block = block;

	for (byte i = 0; i < 6; i++) {
		_key.keyByte[i] = customKey[i];
	}
}

int Read::ReadBlock(MFRC522 reader, byte readBackBlock[])
{
	int largestModulo4Number = _block / 4 * 4;
	int trailerBlock = largestModulo4Number + 3;
	byte buffersize = 18;

	byte status = reader.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &_key, &(reader.uid));
	if (status != MFRC522::STATUS_OK) {
		return 3;
	}

	status = reader.MIFARE_Read(_block, readBackBlock, &buffersize);
	if (status != MFRC522::STATUS_OK) {
		return 4;
	}
}

Write::Write()
{
}

void Write::Initialize(byte block, int cardsToRead)
{
	_block = block;
	_cardsToRead = cardsToRead;

	for (byte i = 0; i < 6; i++) {
		_key.keyByte[i] = 0xFF;
	}
}
void Write::Initialize(byte block, int cardsToRead, byte customKey[])
{
	_block = block;
	_cardsToRead = cardsToRead;

	for (byte i = 0; i < 6; i++) {
		_key.keyByte[i] = customKey[i];
	}
}

void Write::ActivateWriteMode(MFRC522 reader, int cardNames[][9])
{
	String tagID;
	String newTagID;
	String currentCardOnReader;
	byte cardID[16];

	Serial.println(F("---WRITE MODE ACTIVATED---"));
	Serial.println();

	for (int i = 0; i < _cardsToRead; i++) {

		Serial.println(F("Place new card"));
		reader.PCD_Init();

		//Wait for card
		while (!reader.PICC_IsNewCardPresent()) {}

		if (reader.PICC_ReadCardSerial()) {
			newTagID = (reader.uid.uidByte);
		}

		if (tagID != newTagID) {
			for (int h = 0; h < 9; h++) {
				cardID[h] = cardNames[i][h] + 48;
			}

			WriteBlock(reader, cardID);
			Serial.print(F("Writing"));

			VisualDelay(3);
			Serial.println("Success!");
			tagID = newTagID;
			currentCardOnReader = newTagID;
		}
		else {
			tagID = newTagID;
			currentCardOnReader = newTagID;
			i--;
		}

		Serial.println("Remove Card");

		while (tagID == currentCardOnReader) {
			currentCardOnReader = WaitForNewCard(reader);
		}

		if (i < _cardsToRead) {
			Serial.print("Checking For Next Card");
			VisualDelay(3);
			Serial.println();
			Serial.println();
			delay(1000);
		}
	}
	Serial.println("---All Cards Written---");
	Serial.println("Returning to Game...");
	Serial.println();
}

void Write::VisualDelay(int halfSeconds) {
	delay(500);
	for (int i = 0; i < halfSeconds; i++) {
		Serial.print('.');
		delay(500);
	}
}

String Write::WaitForNewCard(MFRC522 reader) {
	reader.PCD_Init();
	if (reader.PICC_IsNewCardPresent() && reader.PICC_ReadCardSerial()) {
		String currentCardID = (reader.uid.uidByte);

		reader.PICC_HaltA();
		reader.PCD_StopCrypto1();

		return currentCardID;
	}

	return "0";
}

int Write::WriteBlock(MFRC522 reader, byte cardID[]) {

	byte status;
	int largestModulo4Number = _block / 4 * 4;
	int trailerBlock = largestModulo4Number + 3;

	if (_block > 2 && (_block + 1) % 4 == 0) {
		Serial.print(_block);
		Serial.println(" is a Trailer Block");
		return 2;
	}

	status = reader.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &_key, &(reader.uid));
	if (status != MFRC522::STATUS_OK) {
		return 3;
	}

	status = reader.MIFARE_Write(_block, cardID, 16);
	if (status != MFRC522::STATUS_OK) {
		reader.PCD_StopCrypto1();
		return 4;
	}

	Serial.println("Configuring card ID");
	reader.PCD_StopCrypto1();
}
