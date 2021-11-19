/* Adapted From:
   WebSocketClientSocketIO.ino
   Created on: 06.06.2016 by Links2004

   Adapted by: SubtleDragon057
   For use with: Project ATEM Duel Disk Proto
*/

#define ESP8266
//#define ESP32

#include <Arduino.h>
#include <ArduinoJson.h>

#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <Hash.h>
#endif

#ifdef ESP32
#include <Wifi.h>
#include <WifiMulti.h>
#include <WiFiClientSecure.h>
#endif

#include <WebSocketsClient.h>
#include <SocketIOclient.h>

#include "src\LocalFunctions.h"
#include "src\Secrets.h"

#ifdef ESP8266
ESP8266WiFiMulti wiFiMulti;
#endif
#ifdef ESP32
WiFiMulti wiFiMulti;
#endif

SocketIOclient socketIO;
LocalFunctions func;
SECRETS secrets;

bool isInDuelRoom = false;
String socketID;

int deckList[20] = {
  25652259,
  64788463,
  90876561,
  89631139,
  24140059,
  32207100,
  79965360,
  41356845,
  62325062,
  46052429,
  18036057,
  70924884,
  64428736,
  53162898,
  31102447,
  89386122,
  36629203,
  99785935,
  67987611,
  29654737
};

void socketIOEvent(socketIOmessageType_t type, uint8_t * payload, std::size_t length) {
  switch (type) {
    case sIOtype_DISCONNECT:
      Serial.printf("[IOc] Disconnected!\n");
      break;
    case sIOtype_CONNECT:
      Serial.printf("[IOc] Connected to url: %s\n", payload);
      socketID = socketIO.getSocketId();
      break;
    case sIOtype_EVENT:
      Serial.printf("[IOc] get event: %s\n", payload);
      HandleRecievedEvent(payload);
      break;
    case sIOtype_ERROR:
      Serial.printf("[IOc] get error: %u\n", length);
      break;
  }
}

void setup() {

  Serial.begin(9600);
  func.Initialize(deckList);

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] BOOT WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  // disable AP
  if (WiFi.getMode() & WIFI_AP) {
    WiFi.softAPdisconnect(true);
  }

  wiFiMulti.addAP(secrets.networkName, secrets.networkPass);

  //WiFi.disconnect();
  while (wiFiMulti.run() != WL_CONNECTED) {
      delay(100);
  }

  socketIO.begin(secrets.socketIP, secrets.socketPort);
  socketIO.onEvent(socketIOEvent);
}

void loop() {
  socketIO.loop();

  while (!isInDuelRoom) {
      if (!Serial.available()) return;

      String data = Serial.readString();

      if (data == "create" || data == "Create") {
          isInDuelRoom = true;

          String output = func.CreateRoom();
          socketIO.sendEVENT(output);
          return;
      }

      if (data == "Join" || data == "join") {
          isInDuelRoom = true;

          Serial.println("Which room would you like to join?");

          while (!Serial.available()) {}

          String roomName = Serial.readString();

          String output = func.JoinRoom(roomName);
          socketIO.sendEVENT(output);
          return;
      }

      Serial.println("Command is Invalid. Valid commands are: Create, Join");
  }
  
  if (Serial.available()) {
	  String data = Serial.readString();

	  String output = func.GetCardEventAsJSON(socketID, data);

	  if (output == NULL) {
          Serial.println("No data available!");
          return;
	  }

      socketIO.sendEVENT(output);
  }
}

void HandleRecievedEvent(uint8_t* payload) {
    String data = func.RecieveEvent(payload);
    
    if (data == "room:close") {
        isInDuelRoom = false;
        return;
    }

    if (data == "room:join") {
        isInDuelRoom = false;
        Serial.println("That room doesn't exist! Please try again");
    }
}
