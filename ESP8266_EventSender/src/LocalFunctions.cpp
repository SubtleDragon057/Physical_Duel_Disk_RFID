#include "Arduino.h"
#include "ArduinoJson.h"
#include "LocalFunctions.h"

LocalFunctions::LocalFunctions()
{
}

String LocalFunctions::GetCardEventAsJSON(String socketID, String data)
{
	String eventInfo[4];
	
	eventInfo[0] = data.substring(0, 1); // Event #
	eventInfo[1] = GetZoneName((data.substring(2, 3)).toInt()); // Zone Name
	eventInfo[2] = data.substring(4, 12); // Serial #
	eventInfo[3] = GetCardPosition(data.substring(13, 14)); // Position

	String output = GetCardEvent(socketID, eventInfo[0].toInt(), eventInfo);

	return output;
}

String LocalFunctions::SummonEvent(String socketID, String eventInfo[]) {

	const std::size_t CAPACITY = JSON_ARRAY_SIZE(2) + 2 * JSON_OBJECT_SIZE(5);
	StaticJsonDocument<CAPACITY> doc;
	JsonArray jsonArray = doc.to<JsonArray>();

	jsonArray.add("card:play");

	JsonObject params = jsonArray.createNestedObject();
	params["duelistId"] = socketID;
	params["cardId"] = eventInfo[2];
	params["copyNumber"] = 1;
	params["zoneName"] = eventInfo[1];
	params["cardPosition"] = eventInfo[3];

	String output;
	serializeJson(doc, output);

	return output;
}

//TODO: Implement Remove Card logic
String LocalFunctions::RemoveCardEvent(String socketID, String eventInfo[]) {

	const std::size_t CAPACITY = JSON_ARRAY_SIZE(2) + 2 * JSON_OBJECT_SIZE(5);
	StaticJsonDocument<CAPACITY> doc;
	JsonArray jsonArray = doc.to<JsonArray>();

	jsonArray.add("card:play");

	JsonObject params = jsonArray.createNestedObject();
	params["duelistId"] = socketID;
	params["cardId"] = eventInfo[2];
	params["copyNumber"] = 1;
	params["zoneName"] = "hand";
	params["cardPosition"] = "faceUp";

	String output;
	serializeJson(doc, output);

	return output;
}

//Private Functions
String LocalFunctions::GetZoneName(int zoneNumber) {
	String zoneName;

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

String LocalFunctions::GetCardPosition(String positionNumber) {
	String cardPosition = "faceUp";

	if (positionNumber == "2") {
		return cardPosition = "faceDownDefence";
	}
	else if (positionNumber == "3") {
		return cardPosition = "faceUpDefence";
	}
	else if (positionNumber == "4") {
		return cardPosition = "faceDown";
	}

	return cardPosition;
}

String LocalFunctions::GetCardEvent(String socketID, int eventName, String eventInfo[]) {
	String output;

	switch (eventName) {
	case 1:
		output = SummonEvent(socketID, eventInfo);
		break;
	case 2:
		output = RemoveCardEvent(socketID, eventInfo);
		break;
	}

	return output;
}
