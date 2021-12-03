#include "Lobby.h"
#include "Arduino.h"
#include "ArduinoJson.h"

Lobby::Lobby() 
{
}
String Lobby::CurrentRoom;

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

    return "NoAction";
}

void Lobby::UpdateCurrentRoom(String roomName) {
    CurrentRoom = roomName;
}

String Lobby::HandleCreateRoomEvent(int deckList[]) {
    StaticJsonDocument<770> staticDoc;
    JsonArray deck = staticDoc.createNestedArray();
    for (int i = 0; i < 36; i++) {
        if (deckList[i] == 0) continue;
        deck.add(deckList[i]);
    }
    Serial.println(deck);

    StaticJsonDocument<770> doc;
    JsonArray jsonArray = doc.to<JsonArray>();

    jsonArray.add("room:create");

    JsonObject params = jsonArray.createNestedObject();
    params["roomName"] = "";
    params["deckList"] = deck;

    String output;
    serializeJson(doc, output);
    serializeJsonPretty(doc, Serial);
    return output;
}

String Lobby::HandleJoinRoomEvent(int deckList[]) {
    Serial.println("Which room would you like to join?");

    while (!Serial.available()) {}

    String roomName = Serial.readString();

    StaticJsonDocument<770> staticDoc;
    JsonArray deck = staticDoc.createNestedArray();
    for (int i = 0; i < 35; i++) {
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