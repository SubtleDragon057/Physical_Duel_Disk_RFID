#include "JSONUtility.h"
#include "ArduinoJson.h"

JSONUtility::JSONUtility() 
{
}

String JSONUtility::HandleCreateRoomEvent(int deckList[]) {
	StaticJsonDocument<770> staticDoc;
	JsonArray deck = staticDoc.createNestedArray();
	for (byte i = 0; i < 35; i++) {
		if (deckList[i] == 0) continue;
		deck.add(deckList[i]);
	}

	StaticJsonDocument<770> doc;
	JsonArray jsonArray = doc.to<JsonArray>();

	jsonArray.add("room:create");

	JsonObject params = jsonArray.createNestedObject();
	params["roomName"] = "";
	params["deckList"] = deck;

	String output;
	serializeJson(doc, output);

	return output;
}

String JSONUtility::HandleJoinRoomEvent(int deckList[]) {
	Serial.printf("Which room would you like to join?\n");

	while (!Serial.available()) {}

	String roomName = Serial.readString();

	StaticJsonDocument<770> staticDoc;
	JsonArray deck = staticDoc.createNestedArray();
	for (byte i = 0; i < 35; i++) {
		if (deckList[i] == 0) continue;
		deck.add(deckList[i]);
	}

	StaticJsonDocument<770> doc;
	JsonArray jsonArray = doc.to<JsonArray>();

	jsonArray.add("room:join");

	JsonObject params = jsonArray.createNestedObject();
	params["roomName"] = roomName;
	params["deckList"] = deck;
	params["error"];

	String output;
	serializeJson(doc, output);
	return output;
}

String JSONUtility::HandleCloseRoomEvent(String roomName) {
	StaticJsonDocument<100> doc;
	JsonArray jsonArray = doc.to<JsonArray>();

	jsonArray.add("room:close");

	JsonObject params = jsonArray.createNestedObject();
	params["roomName"] = roomName;

	String output;
	serializeJson(doc, output);
	return output;
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
	params["zoneName"] = zoneName;
	params["cardPosition"] = cardPosition;

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