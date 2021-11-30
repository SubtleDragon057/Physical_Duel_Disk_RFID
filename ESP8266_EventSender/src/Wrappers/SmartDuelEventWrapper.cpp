#include "Arduino.h"
#include "SmartDuelEventWrapper.h"
#include "ArduinoJson.h"

SmartDuelEventWrapper::SmartDuelEventWrapper() 
{
}

String SmartDuelEventWrapper::GetCardEventAsJSON(String socketID, String data)
{
	String eventInfo[4];

	eventInfo[0] = data.substring(0, 1); // Event #
	eventInfo[1] = GetZoneName(data.substring(1, 2), eventInfo[0]); // Zone Name
	eventInfo[2] = data.substring(2, 10); // Serial #
	eventInfo[3] = GetCardPosition(data.substring(10, 11)); // Position

	return PlayCardEvent(socketID, eventInfo);
}

String SmartDuelEventWrapper::GetAttackEventAsJSON(String socketID, int monsterID, int copyNumber, String targetZone)
{
	const std::size_t CAPACITY = JSON_ARRAY_SIZE(2) + 2 * JSON_OBJECT_SIZE(4);
	StaticJsonDocument<CAPACITY> doc;
	JsonArray jsonArray = doc.to<JsonArray>();

	jsonArray.add("card:attack");

	JsonObject params = jsonArray.createNestedObject();
	params["duelistId"] = socketID;
	params["cardId"] = monsterID;
	params["copyNumber"] = copyNumber;
	params["zoneName"] = targetZone;

	String output;
	serializeJson(doc, output);

	return output;
}

String SmartDuelEventWrapper::GetDeclareEventAsJSON(String socketID, int cardID, int copyNumber) {
	const std::size_t CAPACITY = JSON_ARRAY_SIZE(2) + 2 * JSON_OBJECT_SIZE(5);
	StaticJsonDocument<CAPACITY> doc;
	JsonArray jsonArray = doc.to<JsonArray>();

	jsonArray.add("card:declare");

	JsonObject params = jsonArray.createNestedObject();
	params["duelistId"] = socketID;
	params["cardId"] = cardID;
	params["copyNumber"] = copyNumber;
	params["zoneName"] = nullptr;
	params["cardPosition"] = nullptr;

	String output;
	serializeJson(doc, output);

	return output;
}

String SmartDuelEventWrapper::PlayCardEvent(String socketID, String eventInfo[]) {
	
	const std::size_t CAPACITY = JSON_ARRAY_SIZE(2) + 2 * JSON_OBJECT_SIZE(5);
	StaticJsonDocument<CAPACITY> doc;
	JsonArray jsonArray = doc.to<JsonArray>();

	jsonArray.add("card:play");

	JsonObject params = jsonArray.createNestedObject();
	params["duelistId"] = socketID;
	params["cardId"] = GetCardID(eventInfo[2]);
	params["copyNumber"] = 1;
	params["zoneName"] = eventInfo[1];
	params["cardPosition"] = eventInfo[3];

	String output;
	serializeJson(doc, output);

	return output;
}

String SmartDuelEventWrapper::GetZoneName(String zone, String eventType) {
	if (eventType == "2") {
		return "graveyard";
	}
	
	String zoneName;
	int zoneNumber = zone.toInt();

	switch (zoneNumber) {
	case 0:
		zoneName = "mainMonster1";
		break;
	case 1:
		zoneName = "mainMonster2";
		break;
	case 2:
		zoneName = "mainMonster3";
		break;
	case 3:
		zoneName = "spellTrap1";
		break;
	case 4:
		zoneName = "spellTrap2";
		break;
	case 5:
		zoneName = "spellTrap3";
		break;
	}

	return zoneName;
}

String SmartDuelEventWrapper::GetTargetZoneName(int zoneNumber) {
	String zoneName = "";
	switch (zoneNumber) {
	case 0:
		zoneName = "mainMonster3";
		break;
	case 1:
		zoneName = "mainMonster2";
		break;
	case 2:
		zoneName = "mainMonster1";
		break;
	}

	return zoneName;
}

String SmartDuelEventWrapper::GetCardPosition(String positionNumber) {
	String cardPosition = "faceUp";

	if (positionNumber == "2") {
		return cardPosition = "faceDown";
	}
	else if (positionNumber == "3") {
		return cardPosition = "faceUpDefence";
	}
	else if (positionNumber == "4") {
		return cardPosition = "faceDownDefence";
	}

	return cardPosition;
}

int SmartDuelEventWrapper::GetCardID(String cardID) {
	char charArray[9];
	cardID.toCharArray(charArray, 9);

	return atoi(&charArray[0]);
}