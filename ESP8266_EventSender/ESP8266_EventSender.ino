/* Adapted From:
   WebSocketClientSocketIO.ino
   Created on: 06.06.2016 by Links2004

   Adapted by: SubtleDragon057
   For use with: Project ATEM Duel Disk Proto
*/

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsClient.h>
#include <SocketIOclient.h>
#include <Hash.h>

#include "LocalFunctions.h"

ESP8266WiFiMulti WiFiMulti;
SocketIOclient socketIO;
LocalFunctions func;

String socketIP = "";
int socketPort = 8080;
char* networkName = "Name";
char* networkPass = "Pass";

String eventInfo[6] = {
  "eventType",
  "zoneName",
  "cardID",
  "battleMode",
  "faceMode",
  "position"
};

int deckList[3] = {
  25652259,
  64788463,
  90876561
};


void socketIOEvent(socketIOmessageType_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case sIOtype_DISCONNECT:
      Serial.printf("[IOc] Disconnected!\n");
      break;
    case sIOtype_CONNECT:
      Serial.printf("[IOc] Connected to url: %s\n", payload);

      // join default namespace (no auto join in Socket.IO V3)
      socketIO.send(sIOtype_CONNECT, "/");
      break;
    case sIOtype_EVENT:
      Serial.printf("[IOc] get event: %s\n", payload);
      break;
    case sIOtype_ACK:
      Serial.printf("[IOc] get ack: %u\n", length);
      hexdump(payload, length);
      break;
    case sIOtype_ERROR:
      Serial.printf("[IOc] get error: %u\n", length);
      hexdump(payload, length);
      break;
    case sIOtype_BINARY_EVENT:
      Serial.printf("[IOc] get binary: %u\n", length);
      hexdump(payload, length);
      break;
    case sIOtype_BINARY_ACK:
      Serial.printf("[IOc] get binary ack: %u\n", length);
      hexdump(payload, length);
      break;
  }
}

void setup() {

  Serial.begin(9600);
  func.Begin(deckList);

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] BOOT WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  // disable AP
  if (WiFi.getMode() & WIFI_AP) {
    WiFi.softAPdisconnect(true);
  }

  WiFiMulti.addAP(networkName, networkPass);

  //WiFi.disconnect();
  while (WiFiMulti.run() != WL_CONNECTED) {
    delay(100);
  }

  String ip = WiFi.localIP().toString();
  Serial.printf("[SETUP] WiFi Connected %s\n", ip.c_str());

  socketIO.begin(socketIP, socketPort);
  socketIO.onEvent(socketIOEvent);
  func.CreateRoom();
}

void loop() {
  socketIO.loop();

  if (Serial.available()) {
    String data = Serial.readString();

    eventInfo[0] = data.substring(0, 1);
    eventInfo[1] = func.GetZoneName((data.substring(2, 3)).toInt());
    eventInfo[2] = data.substring(4, 12);
    eventInfo[3] = data.substring(13, 14);
    eventInfo[4] = data.substring(15, 16);

    eventInfo[5] = func.GetCardPosition(eventInfo);
    String json = GetCardEvent(eventInfo[0].toInt(), eventInfo);
     
    socketIO.sendEVENT(json);
  }
}

String GetCardEvent(int eventName, String eventInfo[]) {
   String event;
   
   switch (eventName) {
    case 1:
      event = func.SummonEvent(eventInfo);
      break;
    case 2:
      event = func.RemoveCardEvent(eventInfo);
      break;
    case 6:
      event = func.CreateRoom();
      break;
         
    return event;
  }
}
