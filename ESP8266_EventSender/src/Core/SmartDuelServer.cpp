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
        SmartDuelServer::isConnected = false;
        break;
    case sIOtype_CONNECT:
        Serial.printf("[IOc] Connected to url: %s\n", payload);
        SmartDuelServer::isConnected = true;
        break;
    case sIOtype_EVENT:
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
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, payload);
    String eventName = doc[0];

    if (error) {
        Serial.print("Error: ");
        Serial.println(error.c_str());
        return;
    }

    String eventData = doc[1]["roomName"];

    if (eventName == "room:join") {
        Serial.println("That room doesn't exist! Please try again");
        return;
    }

    ReturnEventName = eventName;
    ReturnData = eventData;
}