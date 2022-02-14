#include "DuelRoom.h"
#include "ArduinoJson.h"

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
    params["roomName"] = RoomName;

    String output;
    serializeJson(doc, output);
    return output;
}

void DuelRoom::UpdateCurrentRoom(String roomName) {
    RoomName = roomName;
}