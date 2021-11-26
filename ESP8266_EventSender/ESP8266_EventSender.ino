/* Adapted From:
   WebSocketClientSocketIO.ino
   Created on: 06.06.2016 by Links2004

   Adapted by: SubtleDragon057
   For use with: Project ATEM Duel Disk Proto
*/

const bool debug = true;

#define esp8266
//#define esp32

#include <Arduino.h>
#include <Wire.h>

#ifdef esp8266
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <Hash.h>
#endif

#ifdef esp32
#include <Wifi.h>
#include <WifiMulti.h>
#include <WiFiClientSecure.h>
#endif

#include "src\LocalFunctions.h"
#include "src\ButtonHandler.h"
#include "src\SmartDuelEventHandler.h"
#include "src\SerialEventHandler.h"
#include "src\Secrets.h"
#include "src\Core\Entities\Button.h"

#ifdef esp8266
ESP8266WiFiMulti wiFiMulti;
#endif
#ifdef esp32
WiFiMulti wiFiMulti;
#endif

ButtonHandler buttonHandler(debug);
SmartDuelEventHandler smartDuelEventHandler(debug);
SerialEventHandler serialEventHandler(debug);
LocalFunctions func;
SECRETS secrets;

String socketID;

const byte numButtons = 5;
Button buttons[numButtons] = {
    Button("Button1", 0),
    Button("Button2", 2),
    Button("Button3", 14),
    Button("Button4", 12),
    Button("Button5", 13)
};

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

void setup() {    

  Serial.begin(9600);
  Wire.begin(4, 5);

  serialEventHandler.Initialize();
  buttonHandler.Initialize(numButtons, buttons);

  // disable AP
  if (WiFi.getMode() & WIFI_AP) {
    WiFi.softAPdisconnect(true);
  }

  wiFiMulti.addAP(secrets.networkName, secrets.networkPass);

  //WiFi.disconnect();
  while (wiFiMulti.run() != WL_CONNECTED) {
      delay(100);
  }

  smartDuelEventHandler.InitializeLobby(deckList);
  smartDuelEventHandler.Connect(secrets.socketIP, secrets.socketPort);
}

void loop() {
  
  while (!smartDuelEventHandler.IsInDuelRoom) {      
      smartDuelEventHandler.ListenToServer();
      buttonHandler.CheckButtons();
      smartDuelEventHandler.HandleLobby(buttonHandler.ButtonEvents);
  }

  if (socketID == NULL) {
      socketID = smartDuelEventHandler.GetSocketId();
      Serial.print("Socket ID: ");
      Serial.println(socketID);
  }

  while (smartDuelEventHandler.IsInDuelRoom && !smartDuelEventHandler.IsDueling) {
      smartDuelEventHandler.ListenToServer();
      buttonHandler.CheckButtons();
      smartDuelEventHandler.HandleDuelRoom(buttonHandler.ButtonEvents);
  }
  
  smartDuelEventHandler.ListenToServer();
  buttonHandler.CheckButtons();
  
  if (Serial.available()) {
	  String data = Serial.readString();
      Serial.println(data);

	  String output = func.GetCardEventAsJSON(socketID, data);

	  if (output == NULL) {
          Serial.println("No data available!");
          return;
	  }

      smartDuelEventHandler.SendEvent(output);
  }
}