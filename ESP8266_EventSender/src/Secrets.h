#ifndef _SECRETS_h
#define _SECRETS_h
#include "arduino.h"

class SECRETS 
{
public:
	String socketIP = "";
	int socketPort = 8080;
	const char* networkName = "";
	const char* networkPass = "";
};

#endif
