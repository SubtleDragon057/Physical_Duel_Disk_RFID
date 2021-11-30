#include "SmartDuelServer.h"
#include "Arduino.h"
#include "WebSocketsClient.h"
#include "SocketIOclient.h"
#include "ArduinoJson.h"

SmartDuelServer::SmartDuelServer() 
{
}
bool SmartDuelServer::isConnected = false;
String SmartDuelServer::ReturnEventName;
String SmartDuelServer::ReturnData;
String SmartDuelServer::DuelistID1;
String SmartDuelServer::DuelistID2;
int SmartDuelServer::CardID;
int SmartDuelServer::CopyNumber;
String SmartDuelServer::ZoneName;

void SmartDuelServer::socketIOEvent(socketIOmessageType_t type, uint8_t* payload, std::size_t length) {
    switch (type) {
    case sIOtype_DISCONNECT:
        Serial.printf("[IOc] Disconnected!\n");
        SmartDuelServer::isConnected = false;
        break;
    case sIOtype_CONNECT:
        Serial.printf("[IOc] Connected to url: %s\n", payload);
        SmartDuelServer::isConnected = true;
        break;
    case sIOtype_EVENT:
        Serial.printf("[IOc] Event: %s\n", payload);
        HandleRecievedEvent(payload);
        break;
    case sIOtype_ERROR:
        Serial.printf("[IOc] get error: %u\n", length);
        break;
    }
}

void SmartDuelServer::Initialize(String socketIP, int socketPort) {
    socketIO.begin(socketIP, socketPort);
    socketIO.onEvent(socketIOEvent);
}

void SmartDuelServer::ListenToServer() {
    socketIO.loop();
}

void SmartDuelServer::SendEvent(String eventData) {
    socketIO.sendEVENT(eventData);
}

String SmartDuelServer::GetSocketId()
{
    return socketIO.getSocketId();
}

void SmartDuelServer::HandleRecievedEvent(uint8_t* payload) {
    
    DynamicJsonDocument doc(35000);
    DeserializationError error = deserializeJson(doc, payload);
    String eventName = doc[0];

    if (error) {
        Serial.print("Error: ");
        Serial.println(error.c_str());
        Serial.print("Memory: ");
        Serial.println(doc.memoryUsage());
        Serial.print("Capacity: ");
        Serial.println(doc.capacity());
        Serial.print("Overflow: ");
        Serial.println(doc.overflowed());
        delay(75);

        doc.clear();
        doc.garbageCollect();
        return;
    }

    String eventScope = eventName.substring(0, 4);
    if (eventScope == "room") {
        String roomEventData = doc[1]["roomName"];

        if (eventName == "room:join") {
            Serial.println("That room doesn't exist! Please try again");
            return;
        }
        
        if (eventName == "room:start") {
            String socket1 = doc[1]["duelRoom"]["duelists"][0]["id"];
            String socket2 = doc[1]["duelRoom"]["duelists"][1]["id"];

            DuelistID1 = socket1;
            DuelistID2 = socket2;
        }

        ReturnEventName = eventName;
        ReturnData = roomEventData;
        return;
    }
    else if (eventScope == "card") {
        String duelistID = doc[1]["duelistId"];
        String cardID = doc[1]["cardId"];
        String copyNum = doc[1]["copyNumber"];
        String zoneName = doc[1]["zoneName"];

        ReturnEventName = eventName;
        DuelistID1 = duelistID;
        CardID = GetIntValue(cardID);
        CopyNumber = GetIntValue(copyNum);
        ZoneName = zoneName;
    }    
}

int SmartDuelServer::GetIntValue(String stringToChange) {
    char charArray[9];
    stringToChange.toCharArray(charArray, 9);

    return atoi(&charArray[0]);
}