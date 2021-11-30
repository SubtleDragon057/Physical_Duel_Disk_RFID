#ifndef SmartDuelServer_h
#define SmartDuelServer_h
#include "Arduino.h"
#include "WebSocketsClient.h"
#include "SocketIOclient.h"
#include "ArduinoJson.h"

class SmartDuelServer {
private:
	SocketIOclient socketIO;

	bool _debug = true;

	static void socketIOEvent(socketIOmessageType_t type, uint8_t* payload, std::size_t length);
	static void HandleRecievedEvent(uint8_t* payload);
	static int GetIntValue(String stringToChange);

public:
	static bool isConnected;
	static String ReturnEventName;
	static String ReturnData;
	static String DuelistID1;
	static String DuelistID2;
	static int CardID;
	static int CopyNumber;
	static String ZoneName;
	
	SmartDuelServer();

	void Initialize(String socketIP, int socketPort);	
	void ListenToServer();
	void SendEvent(String eventData);
	String GetSocketId();
};

#endif 