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

void SmartDuelServer::socketIOEvent(socketIOmessageType_t type, uint8_t* payload, std::size_t length) {    
    switch (type) {
    case sIOtype_DISCONNECT:
        Serial.printf("[IOc] Disconnected!\n");
        break;
    case sIOtype_CONNECT:
        Serial.printf("[IOc] Connected to url: %s\n", payload);
        HandleRecievedEvent("Connection", payload);
        break;
    case sIOtype_EVENT:
        Serial.printf("[IOc] get event: %s\n", payload);
        HandleRecievedEvent("Event", payload);
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

void SmartDuelServer::HandleRecievedEvent(String type, uint8_t* payload) {
    StaticJsonDocument<200> filter;
    filter["DuelRoom"]["Duelists"]["Duelist"][0] = true;
    filter["DuelRoom"]["Duelists"]["Duelist"][1] = true;
    
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, payload);
    String eventName = doc[0];
    String eventData = doc[1]["roomName"];

    if (error) {
        Serial.println("Error");
        return;
    }

    if (eventName == "room:join") {
        Serial.println("That room doesn't exist! Please try again");
        return;
    }

    ReturnEventName = eventName;
    ReturnData = eventData;
}