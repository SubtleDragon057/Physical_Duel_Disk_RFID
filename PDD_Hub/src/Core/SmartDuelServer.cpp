#include "SmartDuelServer.h"
#include "Entities\Enums.h"

//#define DEBUG_Server

SmartDuelServer::SmartDuelServer() 
{
}
bool SmartDuelServer::isConnected = false;

int SmartDuelServer::EventScope;
int SmartDuelServer::EventAction;

String SmartDuelServer::EventData;
String SmartDuelServer::RoomName;
String SmartDuelServer::DuelistID;
int SmartDuelServer::CardID;
int SmartDuelServer::CopyNumber;
int SmartDuelServer::Position;

void SmartDuelServer::socketIOEvent(socketIOmessageType_t type, uint8_t* payload, std::size_t length) {
    switch (type) {
    case sIOtype_DISCONNECT:
#ifdef DEBUG_Server
        Serial.printf("[IOc] Disconnected!\n");
#endif // DEBUG_Server
        
        SmartDuelServer::isConnected = false;
        break;
    case sIOtype_CONNECT:
#ifdef DEBUG_Server
        Serial.printf("[IOc] Connected to url: %s\n", payload);
#endif // DEBUG_Server
        
        SmartDuelServer::isConnected = true;
        break;
    case sIOtype_EVENT:
#ifdef DEBUG_Server
        Serial.printf("[IOc] Event: %s\n", payload);
#endif // DEBUG_Server

        HandleRecievedEvent(payload);
        break;
    case sIOtype_ERROR:
        Serial.printf("[IOc] Error: %u\n", length);
        break;
    }
}

void SmartDuelServer::Initialize(String socketIP, int socketPort) {
    socketIO.begin(socketIP, socketPort);
    socketIO.onEvent(socketIOEvent);
}

void SmartDuelServer::InitializeSSL(String socketIP, int socketPort) {
    socketIO.beginSSL(socketIP, socketPort);
    socketIO.onEvent(socketIOEvent);
}

void SmartDuelServer::ListenToServer() {
    socketIO.loop();
}

void SmartDuelServer::SendEvent(String eventData) {
    if (eventData == String()) return;    
    socketIO.sendEVENT(eventData);

#ifdef DEBUG_Server
    Serial.println(eventData);
#endif // DEBUG_Server
}

String SmartDuelServer::GetSocketId()
{
    return socketIO.getSocketId();
}

void SmartDuelServer::HandleRecievedEvent(uint8_t* payload) {
    
    DynamicJsonDocument doc(38000);
    DeserializationError error = deserializeJson(doc, payload);
    String eventName = doc[0];

    if (error) {
#ifdef DEBUG_Server
        Serial.print("Error: "); Serial.println(error.c_str());
        Serial.print("Memory: "); Serial.println(doc.memoryUsage());
        Serial.print("Capacity: "); Serial.println(doc.capacity());
        Serial.print("Overflow: "); Serial.println(doc.overflowed());
#endif // DEBUG_Server

        doc.clear();
        doc.garbageCollect();
        return;
    }

    String eventScope = eventName.substring(0, 4);
    if (eventScope == "room") {
        EventScope = Enums::EventScope::Room;
        HandleRoomScope(eventName.substring(5), doc);
    }
    else if (eventScope == "card") {
        EventScope = Enums::EventScope::Card;
        HandleCardScope(eventName.substring(5), doc);
    }
    else if (eventScope == "duel") {
        EventScope = Enums::EventScope::Duelist;
        HandleDuelistScope(eventName.substring(8), doc);
    }
}

void SmartDuelServer::HandleRoomScope(String eventAction, DynamicJsonDocument eventJSON) {
    String roomName = eventJSON[1]["roomName"];

    if (eventAction == "create") {
        EventAction = Enums::EventAction::Create;
        RoomName = roomName;
        return;
    }

    if (eventAction == "join") {
        EventAction = Enums::EventAction::Join;
        Serial.println("That room doesn't exist! Please try again");
        return;
    }

    if (eventAction == "start") {
        String socket1 = eventJSON[1]["duelRoom"]["duelists"][0]["id"];
        String socket2 = eventJSON[1]["duelRoom"]["duelists"][1]["id"];
        String turnDuelist = eventJSON[1]["duelRoom"]["duelPhase"]["duelistId"];

        EventAction = Enums::EventAction::Start;
        DuelistID = socket1;
        EventData = socket2;
        EventData = turnDuelist;
        return;
    }

    if (eventAction == "close") {
        EventAction = Enums::EventAction::Close;
    }
}

void SmartDuelServer::HandleCardScope(String eventAction, DynamicJsonDocument eventJSON) {
    String duelistID = eventJSON[1]["duelistId"];
    String cardID = eventJSON[1]["cardId"];
    String copyNum = eventJSON[1]["copyNumber"];
    String zoneName = eventJSON[1]["zoneName"];
    String position = eventJSON[1]["cardPosition"];

    //EventName = eventName;
    DuelistID = duelistID;
    CardID = GetIntValue(cardID);
    CopyNumber = GetIntValue(copyNum);
    Position = GetCardPosition(position);
    EventData = zoneName;

#ifdef DEBUG_Server
    Serial.printf("Card Event Data:\nID: %s\nCard: %i\nCopy: %i\nPosition: %i\nZone: %s\n",
        DuelistID.c_str(), CardID, CopyNumber, Position, EventData.c_str());
#endif // DEBUG_Server
}

void SmartDuelServer::HandleDuelistScope(String eventAction, DynamicJsonDocument eventJSON) {
    if (eventAction == "end-turn") {
        EventAction = Enums::EventAction::EndTurn;
        return;
    }
    
    if (eventAction == "declare-phase") {
        String duelistID = eventJSON[1]["duelistId"];
        String phase = eventJSON[1]["phase"];

        EventAction = Enums::EventAction::Phase;
        DuelistID = duelistID;
        EventData = phase;
        return;
    }
    
    if (eventAction == "update-lifepoints") {
        String duelistID = eventJSON[1]["duelistId"];
        String lifepoints = eventJSON[1]["lifepoints"];

        EventAction = Enums::EventAction::LifePoints;
        DuelistID = duelistID;
        EventData = lifepoints;
        return;
    }

    String duelistID = eventJSON[1]["duelistId"];
    String result = eventJSON[1]["result"];

    EventAction = eventAction == "flip-coin" 
        ? Enums::EventAction::Coin 
        : Enums::EventAction::Dice;
    DuelistID = duelistID;
    EventData = result;
}

int SmartDuelServer::GetIntValue(String stringToChange) {
    char charArray[9];
    stringToChange.toCharArray(charArray, 9);

    return atoi(&charArray[0]);
}

int SmartDuelServer::GetCardPosition(String position) {
    if (position == "faceDown") return 2;
    else if (position == "faceUpDefence") return 3;
    else if (position == "faceDownDefence") return 4;

    return 1;
}