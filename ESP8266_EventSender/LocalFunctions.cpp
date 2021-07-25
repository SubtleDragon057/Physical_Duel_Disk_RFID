#include "Arduino.h"
#include "ArduinoJSON.h"
#include "LocalFunctions.h"

LocalFunctions::LocalFunctions()
{
}

void LocalFunctions::Begin(int deckList[]) {
	_decklist = deckList;
}

String LocalFunctions::GetCardPosition(String eventInfo[]) {
	String cardPosition = "faceUp";

	if (eventInfo[3] == "0" && eventInfo[4] == "0") {
		return cardPosition = "faceDownDefence";
	}
	else if (eventInfo[3] == "0" && eventInfo[4] == "1") {
		return cardPosition = "faceUpDefence";
	}
	else if (eventInfo[3] == "1" && eventInfo[4] == "0") {
		return cardPosition = "faceDown";
	}

	return cardPosition;
}

String LocalFunctions::GetZoneName(int zoneNumber) {
	String zoneName;

	switch (zoneNumber) {
	case 1:
		zoneName = "mainMonster1";
		break;
	case 2:
		zoneName = "mainMonster2";
		break;
	case 3:
		zoneName = "mainMonster3";
		break;
	case 4:
		zoneName = "spellTrap1";
		break;
	case 5:
		zoneName = "spellTrap2";
		break;
	case 6:
		zoneName = "spellTrap3";
		break;
	}

	return zoneName;
}

String LocalFunctions::SummonEvent(String eventInfo[]) {

	const size_t CAPACITY = JSON_ARRAY_SIZE(2) + 4 * JSON_OBJECT_SIZE(2);
	StaticJsonDocument<CAPACITY> doc;
	JsonArray jsonArray = doc.to<JsonArray>();

	jsonArray.add("card:play");

	JsonObject params = jsonArray.createNestedObject();
	params["cardId"] = eventInfo[2];
	params["zoneName"] = eventInfo[1];
	params["cardPosition"] = eventInfo[5];

	String output;
	serializeJson(doc, output);

	return output;
}

String LocalFunctions::RemoveCardEvent(String eventInfo[]) {

	const size_t CAPACITY = JSON_ARRAY_SIZE(2) + 3 * JSON_OBJECT_SIZE(2);
	StaticJsonDocument<CAPACITY> doc;
	JsonArray jsonArray = doc.to<JsonArray>();

	jsonArray.add("card:remove");

	JsonObject params = jsonArray.createNestedObject();
	params["zoneName"] = eventInfo[1];

	String output;
	serializeJson(doc, output);

	return output;
}

String LocalFunctions::CreateRoom() {
	const size_t CAPACITY = JSON_ARRAY_SIZE(2) + 2 * JSON_OBJECT_SIZE(2);
	StaticJsonDocument<CAPACITY> doc;
	JsonArray jsonArray = doc.to<JsonArray>();

	jsonArray.add("room:create");

	JsonArray activeDeck = jsonArray.createNestedArray();
	for (int i = 0; i < (sizeof(_decklist) / sizeof(_decklist[0])); i++) {
		activeDeck.add(_decklist[i]);
	}

	JsonObject params = jsonArray.createNestedObject();
	params["roomName"] = NULL;
	params["deckList"] = activeDeck;

	String output;
	serializeJson(doc, output);

	return output;
}
