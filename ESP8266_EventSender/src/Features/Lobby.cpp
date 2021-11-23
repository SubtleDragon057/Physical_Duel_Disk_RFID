#include "Lobby.h"
#include "Arduino.h"
#include "ArduinoJson.h"

Lobby::Lobby() 
{
}
String Lobby::CurrentRoom;

void Lobby::Initialize(int decklist[]) {
    _decklist = decklist;
}

String Lobby::CheckLobbyForAction(int buttonEvents[])
{
    String data;
    if (Serial.available()) {
        data = Serial.readString();;
    }

    if (data == "create" || data == "Create" || buttonEvents[0] != 0) {
        return HandleCreateRoomEvent();
    }
    else if (data == "Join" || data == "join" || buttonEvents[1] != 0) {
        return HandleJoinRoomEvent();
    }

    return "NoAction";
}

void Lobby::UpdateCurrentRoom(String roomName) {
    CurrentRoom = roomName;
}

String Lobby::HandleCreateRoomEvent() {
    StaticJsonDocument<384> staticDoc;
    JsonArray deck = staticDoc.createNestedArray();
    for (int i = 0; i < 20; i++) {
        deck.add(_decklist[i]);
    }

    StaticJsonDocument<512> doc;
    JsonArray jsonArray = doc.to<JsonArray>();

    jsonArray.add("room:create");

    JsonObject params = jsonArray.createNestedObject();
    params["roomName"] = "";
    params["deckList"] = deck;

    String output;
    serializeJson(doc, output);
    return output;
}

String Lobby::HandleJoinRoomEvent() {
    Serial.println("Which room would you like to join?");

    while (!Serial.available()) {}

    String roomName = Serial.readString();

    StaticJsonDocument<384> dynamicDoc;
    JsonArray deck = dynamicDoc.createNestedArray();
    for (int i = 0; i < 20; i++) {
        deck.add(_decklist[i]);
    }

    StaticJsonDocument<512> doc;
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