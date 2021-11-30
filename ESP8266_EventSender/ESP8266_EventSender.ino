/* Adapted From:
   WebSocketClientSocketIO.ino
   Created on: 06.06.2016 by Links2004

   Adapted by: SubtleDragon057
   For use with: Project ATEM Duel Disk Proto
*/

const bool debug = false;

//#define esp8266
#define esp32

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

#include "src\ButtonHandler.h"
#include "src\SmartDuelEventHandler.h"
#include "src\CommunicationsHandler.h"
#include "src\Secrets.h"
#include "src\Core\Entities\Button.h"

#ifdef esp8266
ESP8266WiFiMulti wiFiMulti;
byte sdaPin = 4;
byte sclPin = 5;
byte button1Pin = 0;
byte button2Pin = 2;
byte button3Pin = 14;
byte button4Pin = 12;
byte button5Pin = 13;
#endif
#ifdef esp32
WiFiMulti wiFiMulti;
byte sdaPin = 21;
byte sclPin = 22;
byte button1Pin = 19;
byte button2Pin = 18;
byte button3Pin = 5;
byte button4Pin = 17;
byte button5Pin = 16;
#endif

ButtonHandler buttonHandler(debug);
SmartDuelEventHandler smartDuelEventHandler(debug);
CommunicationsHandler communicationsHandler(debug);
SECRETS secrets;

const byte numButtons = 5;
Button buttons[numButtons] = {
    Button("Button1", button1Pin),
    Button("Button2", button2Pin),
    Button("Button3", button3Pin),
    Button("Button4", button4Pin),
    Button("Button5", button5Pin)
};

int deckList[35] = {
  25652259,
  11549357,
  90876561,
  62651957,
  84636823,
  65622692,
  98502113,
  5818798,
  11321183,
  14898066,
  71413901,
  64500000,
  99785935,
  46986414,
  76909279,
  4796100,
  78193831,
  89631139,
  46363422,
  31553716,
  39256679,
  91998119,
  75347539,
  77207191
};

void setup() {    

  Serial.begin(9600);
  Wire.begin(sdaPin, sclPin);

  communicationsHandler.Initialize();
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

  while (smartDuelEventHandler.IsInDuelRoom && !smartDuelEventHandler.IsDueling) {
      smartDuelEventHandler.ListenToServer();
      buttonHandler.CheckButtons();
      smartDuelEventHandler.HandleDuelRoom(buttonHandler.ButtonEvents);
  }
  
  smartDuelEventHandler.ListenToServer();
  buttonHandler.CheckButtons();
  smartDuelEventHandler.HandleButtonInteraction(buttonHandler.ButtonEvents);

  while (!smartDuelEventHandler.HasAttackTarget) {
      smartDuelEventHandler.ListenToServer();
      buttonHandler.CheckButtons();
      smartDuelEventHandler.HandleAttackEvent(buttonHandler.ButtonEvents);
  }
  
  String output = communicationsHandler.PollForNewEvent();
  if (output == "No Events!!") return;

  smartDuelEventHandler.HandleOutgoingEvent(output);
}