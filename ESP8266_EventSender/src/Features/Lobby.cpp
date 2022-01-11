#include "Lobby.h"
#include "ArduinoJson.h"

Lobby::Lobby() 
{
}

String Lobby::CheckLobbyForAction(int buttonEvents[], int deckList[])
{
    String data;
    if (Serial.available()) {
        data = Serial.readString();;
    }

    if (data == "create" || data == "Create" || buttonEvents[0] != 0) {
        return HandleCreateRoomEvent(deckList);
    }
    else if (data == "Join" || data == "join" || buttonEvents[1] != 0) {
        return HandleJoinRoomEvent(deckList);
    }
    else if (buttonEvents[5] == 9) { // Multi34 Button Event (ie. Hold Buttons 4 & 5)
        return "writeMode";
    }

    return "NoAction";
}

String Lobby::HandleCreateRoomEvent(int deckList[]) {
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

String Lobby::HandleJoinRoomEvent(int deckList[]) {
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