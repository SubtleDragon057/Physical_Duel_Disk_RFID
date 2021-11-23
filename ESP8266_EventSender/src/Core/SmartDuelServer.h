#ifndef SmartDuelServer_h
#define SmartDuelServer_h
#include "Arduino.h"
#include "WebSocketsClient.h"
#include "SocketIOclient.h"
#include "ArduinoJson.h"

class SmartDuelServer {
private:
	SocketIOclient socketIO;

	static void socketIOEvent(socketIOmessageType_t type, uint8_t* payload, std::size_t length);
	static void HandleRecievedEvent(String type, uint8_t* payload);

public:
	static bool isConnected;
	static String ReturnEventName;
	static String ReturnData;
	
	SmartDuelServer();

	void Initialize(String socketIP, int socketPort);	
	void ListenToServer();
	void SendEvent(String eventData);
	String GetSocketId();
};

#endif 