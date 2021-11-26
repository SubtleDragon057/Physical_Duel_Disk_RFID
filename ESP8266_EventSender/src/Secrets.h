#ifndef _SECRETS_h
#define _SECRETS_h
#include "arduino.h"

class SECRETS 
{
public:
	String socketIP = "192.168.0.114";
	int socketPort = 8080;
	const char* networkName = "Cliff's Landscaping";
	const char* networkPass = "7809808530";
};

#endif
