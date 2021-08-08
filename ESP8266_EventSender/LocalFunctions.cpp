#include "Arduino.h"
#include "ArduinoJSON.h"
#include "LocalFunctions.h"

LocalFunctions::LocalFunctions()
{
}

void LocalFunctions::Begin(int deckList[], String customID) {
	_decklist = deckList;
	_customID = customID;
}

String LocalFunctions::GetCardEventAsJSON(String data)
{
	String eventInfo[6];
	
	eventInfo[0] = data.substring(0, 1); //Event #
	eventInfo[1] = GetZoneName((data.substring(2, 3)).toInt()); //Zone Name
	eventInfo[2] = data.substring(4, 12); //Serial #
	eventInfo[3] = data.substring(13, 14); //Battle Mode
	eventInfo[4] = data.substring(15, 16); //Face Mode
	eventInfo[5] = GetCardPosition(eventInfo); //Position

	String output = GetCardEvent(eventInfo[0].toInt(), eventInfo);

	return output;
}

String LocalFunctions::CreateRoom() {
	StaticJsonDocument<384> dynamicDoc;
	JsonArray deck = dynamicDoc.createNestedArray();
	for (int i = 0; i < 20; i++) {
		deck.add(_decklist[i]);
	}
	Serial.println(deck);

	StaticJsonDocument<512> doc;
	JsonArray jsonArray = doc.to<JsonArray>();

	jsonArray.add("room:create");

	JsonObject params = jsonArray.createNestedObject();
	params["roomName"] = "";
	params["deckList"] = deck;
	params["customDuelistID"] = _customID;

	String output;
	serializeJson(doc, output);

	return output;
}

String LocalFunctions::JoinRoom(String roomName)
{
	StaticJsonDocument<384> dynamicDoc;
	JsonArray deck = dynamicDoc.createNestedArray();
	for (int i = 0; i < 20; i++) {
		deck.add(_decklist[i]);
	}
	Serial.println(deck);

	StaticJsonDocument<512> doc;
	JsonArray jsonArray = doc.to<JsonArray>();

	jsonArray.add("room:join");

	JsonObject params = jsonArray.createNestedObject();
	params["roomName"] = roomName;
	params["deckList"] = deck;
	params["customDuelistID"] = _customID;

	String output;
	serializeJson(doc, output);

	return output;
}

String LocalFunctions::SummonEvent(String eventInfo[]) {

	const std::size_t CAPACITY = JSON_ARRAY_SIZE(2) + 2 * JSON_OBJECT_SIZE(5);
	StaticJsonDocument<CAPACITY> doc;
	JsonArray jsonArray = doc.to<JsonArray>();

	jsonArray.add("card:play");

	JsonObject params = jsonArray.createNestedObject();
	params["duelistId"] = _customID;
	params["cardId"] = eventInfo[2];
	params["copyNumber"] = 1;
	params["zoneName"] = eventInfo[1];
	params["cardPosition"] = eventInfo[6];

	String output;
	serializeJson(doc, output);

	return output;
}

//TODO: Implement Remove Card logic
String LocalFunctions::RemoveCardEvent(String eventInfo[]) {

	const std::size_t CAPACITY = JSON_ARRAY_SIZE(2) + 2 * JSON_OBJECT_SIZE(5);
	StaticJsonDocument<CAPACITY> doc;
	JsonArray jsonArray = doc.to<JsonArray>();

	jsonArray.add("card:remove");

	JsonObject params = jsonArray.createNestedObject();
	params["duelistId"] = _customID;
	params["cardId"] = eventInfo[2];
	params["copyNumber"] = 1;
	params["zoneName"] = eventInfo[1];
	params["cardPosition"] = eventInfo[6];

	String output;
	serializeJson(doc, output);

	return output;
}

String LocalFunctions::RecieveEvent(uint8_t* payload)
{	
	DynamicJsonDocument doc(1024);
	DeserializationError error = deserializeJson(doc, payload);
	if (error) {
		Serial.println("Error");
	}

	String data = doc[0];
	return data;
}

//Private Functions
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

String LocalFunctions::GetCardEvent(int eventName, String eventInfo[]) {
	String output;

	switch (eventName) {
	case 1:
		output = SummonEvent(eventInfo);
		break;
	case 2:
		output = RemoveCardEvent(eventInfo);
		break;
	}

	return output;
}
