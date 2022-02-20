#ifndef SmartDuelServer_h
#define SmartDuelServer_h
#include "Arduino.h"
#include "SocketIOclient.h"
#include "ArduinoJson.h"

class SmartDuelServer {
private:
	SocketIOclient socketIO;

	static void socketIOEvent(socketIOmessageType_t type, uint8_t* payload, std::size_t length);
	static void HandleRecievedEvent(uint8_t* payload);
	static int GetIntValue(String stringToChange);
	static int GetCardPosition(String position);

	// Event Scope Functions
	static void HandleRoomScope(String eventAction, DynamicJsonDocument eventJSON);
	static void HandleCardScope(String eventAction, DynamicJsonDocument eventJSON);
	static void HandleDuelistScope(String eventAction, DynamicJsonDocument eventJSON);

public:
	static bool isConnected;

	static int EventScope;
	static int EventAction;

	static String EventData;
	static String RoomName;
	static String DuelistID;
	static int CardID;
	static int CopyNumber;
	static int Position;
	
	SmartDuelServer();

	void Initialize(String socketIP, int socketPort);
	void InitializeSSL(String socketIP, int socketPort);

	void ListenToServer();
	void SendEvent(String eventData);
	String GetSocketId();
};

#endif 