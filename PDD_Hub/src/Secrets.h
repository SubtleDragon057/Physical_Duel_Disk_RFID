#ifndef _SECRETS_h
#define _SECRETS_h
#include "arduino.h"

class SECRETS {
public:
	String socketIP = "";
	int socketPort = 8080;
	const char* networkName = "";
	const char* networkPass = "";

	String onlineServerAddress = "smart-duel-server.herokuapp.com";
	int onlineServerPort = 80;
	int onlineServerSecurePort = 443;
};

#endif
