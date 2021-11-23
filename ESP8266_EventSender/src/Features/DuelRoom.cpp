#include "DuelRoom.h"
#include "Arduino.h"
#include "ArduinoJson.h"
#include "Lobby.h"

DuelRoom::DuelRoom() 
{
}

String DuelRoom::CheckDuelRoomForAction(int buttonEvents[]) {
    
    String data;
    if (Serial.available()) {
        data = Serial.readString();;
    }    
    
    if (data == "Leave" || data == "Close" || buttonEvents[0] != 0) {
        return HandleCloseRoomEvent();
    }

    return "NoAction";
}

String DuelRoom::HandleCloseRoomEvent() {
    StaticJsonDocument<100> doc;
    JsonArray jsonArray = doc.to<JsonArray>();

    jsonArray.add("room:close");

    JsonObject params = jsonArray.createNestedObject();
    params["roomName"] = Lobby::CurrentRoom;

    String output;
    serializeJson(doc, output);
    return output;
}