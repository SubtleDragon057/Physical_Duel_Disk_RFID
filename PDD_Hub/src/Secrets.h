#ifndef _SECRETS_h
#define _SECRETS_h
#include "arduino.h"

class SECRETS {
public:
	String socketIP = "192.168.1.68";
	int socketPort = 8080;
	const char* networkName = "ThatsNoMoon";
	const char* networkPass = "D34thst4R";

	String onlineServerAddress = "smart-duel-server.herokuapp.com";
	int onlineServerPort = 80;
	int onlineServerSecurePort = 443;
};

#endif
