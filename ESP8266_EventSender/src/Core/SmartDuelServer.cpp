#include "SmartDuelServer.h"
#include "ArduinoJson.h"

SmartDuelServer::SmartDuelServer() 
{
}
bool SmartDuelServer::isConnected = false;

String SmartDuelServer::EventName;
String SmartDuelServer::EventData;
String SmartDuelServer::RoomName;
String SmartDuelServer::DuelistID;
int SmartDuelServer::CardID;
int SmartDuelServer::CopyNumber;

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
#ifdef DEBUG_Server
        Serial.printf("[IOc] Event: %s\n", payload);
#endif // DEBUG_Server

        HandleRecievedEvent(payload);
        break;
    case sIOtype_ERROR:
        Serial.printf("[IOc] Error: %u\n", length);
        break;
    }
}

void SmartDuelServer::Initialize(String socketIP, int socketPort) {
    socketIO.begin(socketIP, socketPort);
    socketIO.onEvent(socketIOEvent);
}

void SmartDuelServer::InitializeSSL(String socketIP, int socketPort) {
    socketIO.beginSSL(socketIP, socketPort);
    socketIO.onEvent(socketIOEvent);
}

void SmartDuelServer::ListenToServer() {
    socketIO.loop();
}

void SmartDuelServer::SendEvent(String eventData) {
    if (eventData == String()) return;    
    socketIO.sendEVENT(eventData);
}

String SmartDuelServer::GetSocketId()
{
    return socketIO.getSocketId();
}

void SmartDuelServer::HandleRecievedEvent(uint8_t* payload) {
    
    DynamicJsonDocument doc(38000);
    DeserializationError error = deserializeJson(doc, payload);
    String eventName = doc[0];

    if (error) {
        /*Serial.print("Error: ");
        Serial.println(error.c_str());
        Serial.print("Memory: ");
        Serial.println(doc.memoryUsage());
        Serial.print("Capacity: ");
        Serial.println(doc.capacity());
        Serial.print("Overflow: ");
        Serial.println(doc.overflowed());*/

        doc.clear();
        doc.garbageCollect();
        return;
    }

    String eventScope = eventName.substring(0, 4);
    if (eventScope == "room") {
        String roomName = doc[1]["roomName"];

        if (eventName == "room:join") {
            Serial.println("That room doesn't exist! Please try again");
            return;
        }
        
        if (eventName == "room:start") {
            String socket1 = doc[1]["duelRoom"]["duelists"][0]["id"];
            String socket2 = doc[1]["duelRoom"]["duelists"][1]["id"];
            String turnDuelist = doc[1]["duelRoom"]["duelPhase"]["duelistId"];

            DuelistID = socket1;
            EventData = socket2;
            EventData = turnDuelist;
        }

        EventName = eventName;
        RoomName = roomName;
    }
    else if (eventScope == "card") {
        String duelistID = doc[1]["duelistId"];
        String cardID = doc[1]["cardId"];
        String copyNum = doc[1]["copyNumber"];
        String zoneName = doc[1]["zoneName"];

        EventName = eventName;
        DuelistID = duelistID;
        CardID = GetIntValue(cardID);
        CopyNumber = GetIntValue(copyNum);
        EventData = zoneName;
    }
    else if (eventScope == "duel") {
        if (eventName == "duelist:declare-phase") {
            String duelistID = doc[1]["duelistId"];
            String phase = doc[1]["phase"];

            EventName = eventName;
            DuelistID = duelistID;
            EventData = phase;
            return;
        }
        
        String duelistID = doc[1]["duelistId"];
        String result = doc[1]["result"];

        EventName = eventName;
        DuelistID = duelistID;
        EventData = result;
    }
}

int SmartDuelServer::GetIntValue(String stringToChange) {
    char charArray[9];
    stringToChange.toCharArray(charArray, 9);

    return atoi(&charArray[0]);
}