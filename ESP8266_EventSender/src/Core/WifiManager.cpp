#include "WifiManager.h"

//#define DEBUG_WM

WifiManager::WifiManager()
{
}

void WifiManager::Connect(const char* networkName, const char* networkPass) {
	// disable AP
	if (WiFi.getMode() & WIFI_AP) {
		WiFi.softAPdisconnect(true);
	}

	wiFiMulti.addAP(networkName, networkPass);

	WiFi.disconnect();
	while (wiFiMulti.run() != WL_CONNECTED) {
		HandleRetry("[ERROR] Wifi Not Connected");
	}

#ifdef DEBUG_WM
	String networkID = WiFi.SSID();
	Serial.printf("\n[SETUP] Wifi Connected: %s\n", networkID.c_str());
#endif // DEBUG_WM

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