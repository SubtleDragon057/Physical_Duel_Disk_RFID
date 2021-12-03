#include "WifiManager.h"
#include "Arduino.h"

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

WifiManager::WifiManager()
{
}

void WifiManager::Connect(const char* networkName, const char* networkPass) {
	// disable AP
	if (WiFi.getMode() & WIFI_AP) {
		WiFi.softAPdisconnect(true);
	}

	wiFiMulti.addAP(networkName, networkPass);

	//WiFi.disconnect();
	while (wiFiMulti.run() != WL_CONNECTED) {
		HandleRetry("[ERROR] Wifi Not Connected");
	}

	String networkID = WiFi.SSID();
	Serial.println();
	Serial.printf("[SETUP] Wifi Connected: %s\n", networkID.c_str());
}

void WifiManager::HandleRetry(String errorMessage) {
	static int i = 0;
	delay(50);
	i++;

	if (i >= 20) {
		Serial.println(errorMessage);
		i = 0;
	}
}