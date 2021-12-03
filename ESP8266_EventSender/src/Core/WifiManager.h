#ifndef WifiManager_h
#define WifiManager_h
#include "Arduino.h"

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

class WifiManager {
private:
#ifdef ESP8266
	ESP8266WiFiMulti wiFiMulti
#endif	
#ifdef ESP32
	WiFiMulti wiFiMulti;
#endif

public:
	WifiManager();

	void Connect(const char* networkName, const char* networkPass);
	void HandleRetry(String errorMessage);
};

#endif