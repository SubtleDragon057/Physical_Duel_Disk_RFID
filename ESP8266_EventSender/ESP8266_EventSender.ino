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

ESP8266WiFiMulti WiFiMulti;
SocketIOclient socketIO;

String socketIP = "";
int socketPort = 8080;

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

  socketIO.begin("192.168.1.64", 8080);
  socketIO.onEvent(socketIOEvent);
  CreateRoom();
}

void loop() {
  socketIO.loop();

  if (Serial.available()) {
    String data = Serial.readString();

    eventInfo[0] = data.substring(0, 1);
    eventInfo[1] = GetZoneName((data.substring(2, 3)).toInt());
    eventInfo[2] = data.substring(4, 12);
    eventInfo[3] = data.substring(13, 14);
    eventInfo[4] = data.substring(15, 16);

    eventInfo[5] = GetCardPosition(eventInfo);
    GetCardEvent(eventInfo[0].toInt(), eventInfo);
  }
}

String GetCardPosition(String eventInfo[]) {
  String cardPosition = "faceUp";

  if(eventInfo[3] == "0" && eventInfo[4] == "0") {
    return cardPosition = "faceDownDefence";
  }
  else if(eventInfo[3] == "0" && eventInfo[4] == "1") {
    return cardPosition = "faceUpDefence";
  }
  else if(eventInfo[3] == "1" && eventInfo[4] == "0") {
    return cardPosition = "faceDown";
  }

  return cardPosition;
}

void GetCardEvent(int eventName, String eventInfo[]) {
  switch (eventName) {
    case 1:
      SummonEvent(eventInfo);
      break;
    case 2:
      RemoveCardEvent(eventInfo);
      break;
    case 3:
      PositionChangeEvent(eventInfo);
      break;
    case 4:
      SetSpellEvent(eventInfo);
      break;
    case 5:
      //Activate Spell Event
      break;
    case 6:
      CreateRoom();
      break;
  }
}

String GetZoneName(int zoneNumber) {
  String zoneName;

  switch (zoneNumber) {
    case 1:
      zoneName = "mainMonster1";
      break;
    case 2:
      zoneName = "mainMonster2";
      break;
    case 3:
      zoneName = "mainMonster3";
      break;
    case 4:
      zoneName = "spellTrap1";
      break;
    case 5:
      zoneName = "spellTrap2";
      break;
    case 6:
      zoneName = "spellTrap3";
      break;
  }

  return zoneName;
}

void SummonEvent(String eventInfo[]) {

  const size_t CAPACITY = JSON_ARRAY_SIZE(2) + 4 * JSON_OBJECT_SIZE(2);
  StaticJsonDocument<CAPACITY> doc;
  JsonArray array = doc.to<JsonArray>();

  array.add("card:play");

  JsonObject params = array.createNestedObject();
  params["cardId"] = eventInfo[2];
  params["zoneName"] = eventInfo[1];
  params["cardPosition"] = eventInfo[5];

  String output;
  serializeJson(doc, output);

  socketIO.sendEVENT(output);
}

void RemoveCardEvent(String eventInfo[]) {

  const size_t CAPACITY = JSON_ARRAY_SIZE(2) + 3 * JSON_OBJECT_SIZE(2);
  StaticJsonDocument<CAPACITY> doc;
  JsonArray array = doc.to<JsonArray>();

  array.add("card:remove");

  JsonObject params = array.createNestedObject();
  params["zoneName"] = eventInfo[1];
  //params["battleMode"] = eventInfo[3];
  //params["faceMode"] = eventInfo[4];

  String output;
  serializeJson(doc, output);

  socketIO.sendEVENT(output);
  Serial.println(output);
}

void PositionChangeEvent(String eventInfo[]) {

  const size_t CAPACITY = JSON_ARRAY_SIZE(2) + 3 * JSON_OBJECT_SIZE(2);
  StaticJsonDocument<CAPACITY> doc;
  JsonArray array = doc.to<JsonArray>();

  array.add("positionChangeEvent");

  JsonObject params = array.createNestedObject();
  params["zoneName"] = eventInfo[1];
  params["battleMode"] = eventInfo[3];
  params["faceMode"] = eventInfo[4];

  String output;
  serializeJson(doc, output);

  socketIO.sendEVENT(output);
  Serial.println(output);
}

void SetSpellEvent(String eventInfo[]) {

  const size_t CAPACITY = JSON_ARRAY_SIZE(2) + 4 * JSON_OBJECT_SIZE(2);
  StaticJsonDocument<CAPACITY> doc;
  JsonArray array = doc.to<JsonArray>();

  array.add("spellTrapSetEvent");

  JsonObject params = array.createNestedObject();
  params["cardId"] = eventInfo[2];
  params["zoneName"] = eventInfo[1];
  params["setMode"] = eventInfo[3];
  params["extraMode"] = "0";

  String output;
  serializeJson(doc, output);

  socketIO.sendEVENT(output);
}

void CreateRoom() {
  const size_t CAPACITY = JSON_ARRAY_SIZE(2) + 2 * JSON_OBJECT_SIZE(2);
  StaticJsonDocument<CAPACITY> doc;
  JsonArray array = doc.to<JsonArray>();

  array.add("room:create");
  
  JsonArray decklist = array.createNestedArray();
  for(int i = 0; i < (sizeof(deckList)/sizeof(deckList[0])); i++) {
    decklist.add(deckList[i]);
  }

  JsonObject params = array.createNestedObject();
  params["roomName"] = NULL;
  params["deckList"] = decklist;

  String output;
  serializeJson(doc, output);

  socketIO.sendEVENT(output);
}
