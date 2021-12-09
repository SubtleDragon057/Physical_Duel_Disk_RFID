#include "JSONUtility.h"
#include "ArduinoJson.h"

JSONUtility::JSONUtility() 
{
}

String JSONUtility::GetCardEventFromArduino(String socketID, String data)
{
	String eventName = data.substring(0, 1); // Event #
	String zoneName = GetZoneName(data.substring(1, 2), eventName); // Zone Name
	int cardID = GetCardID(data.substring(2, 10)); // Serial #
	int copyNumber = 1; // TODO: Implement copy number from deck builder
	String cardPosition = GetCardPosition(data.substring(10, 11)); // Position

	return GetCardEventAsJSON(socketID, "card:play", cardID, copyNumber, zoneName, cardPosition);
}

String JSONUtility::GetCardEventAsJSON(String socketID, String eventName, int cardID, int copyNumber, 
	String zoneName, String cardPosition) {
	
	if (eventName == "") return String();

	const std::size_t CAPACITY = JSON_ARRAY_SIZE(2) + 2 * JSON_OBJECT_SIZE(5);
	StaticJsonDocument<CAPACITY> doc;
	JsonArray jsonArray = doc.to<JsonArray>();

	jsonArray.add(eventName);

	JsonObject params = jsonArray.createNestedObject();
	params["duelistId"] = socketID;
	params["cardId"] = cardID;
	params["copyNumber"] = copyNumber;
	params["zoneName"] = zoneName == "" ? String() : zoneName;
	params["cardPosition"] = cardPosition == "" ? String() : cardPosition;

	String output;
	serializeJson(doc, output);

	return output;
}

String JSONUtility::GetDuelistEventAsJSON(String socketID, String eventName) {
	
	if (eventName == "") return String();
	
	const std::size_t CAPACITY = JSON_ARRAY_SIZE(2) + 2 * JSON_OBJECT_SIZE(2);
	StaticJsonDocument<CAPACITY> doc;
	JsonArray jsonArray = doc.to<JsonArray>();

	jsonArray.add(eventName);

	JsonObject params = jsonArray.createNestedObject();
	params["duelistId"] = socketID;
	params["result"];

	String output;
	serializeJson(doc, output);

	return output;
}

String JSONUtility::GetPhaseEventAsJSON(String socketId, String eventName, String newPhase) {
	
	const std::size_t CAPACITY = JSON_ARRAY_SIZE(2) + 2 * JSON_OBJECT_SIZE(3);
	StaticJsonDocument<CAPACITY> doc;
	JsonArray jsonArray = doc.to<JsonArray>();

	jsonArray.add(eventName);

	if (newPhase == "") {
		JsonObject params = jsonArray.createNestedObject();
		params["duelistId"] = socketId;
		params["phase"] = nullptr;
		params["result"] = nullptr;
	}
	else {
		JsonObject params = jsonArray.createNestedObject();
		params["duelistId"] = socketId;
		params["phase"] = newPhase;
		params["result"] = nullptr;
	}

	String output;
	serializeJson(doc, output);

	return output;
}

String JSONUtility::GetZoneName(String zone, String eventType) {
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

String JSONUtility::GetCardPosition(String positionNumber) {
	String cardPosition;
	int positionNum = positionNumber.toInt();

	switch (positionNum)
	{
		case 2:
			cardPosition = "faceDown";
			break;
		case 3:
			cardPosition = "faceUpDefence";
			break;
		case 4:
			cardPosition = "faceDownDefence";
			break;
		default:
			cardPosition = "faceUp";
			break;
	}

	return cardPosition;
}

int JSONUtility::GetCardID(String cardID) {
	char charArray[9];
	cardID.toCharArray(charArray, 9);

	return atoi(&charArray[0]);
}