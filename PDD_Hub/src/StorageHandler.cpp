#include "StorageHandler.h"
#include "SD.h"
#include "Core\Entities\Enums.h"

StorageHandler::StorageHandler(PeripheralsHandler &peripheralsHandler)
{
    _peripheralsHandler = &peripheralsHandler;
}

void StorageHandler::Initialize(bool isSDReaderConnected) {    
    _isSDConnected = isSDReaderConnected;

    if (!_isSDConnected) {
        String text[] = { "Using Default Deck" };
        _peripheralsHandler->Display(PeripheralsHandler::UI_Init, text);
    }
    else {
        String text[] = { "Decks Configured!" };
        _peripheralsHandler->Display(PeripheralsHandler::UI_Init, text);
    }
    delay(2000);
}

void StorageHandler::ChooseDeck(int buttonEvents[]) {
    if (!_isSDConnected) {
        UseDefaultDeck();
        return;
    }
    
    _deckName = GetDeckName(buttonEvents);
    if (_deckName == String()) return;
    
    ReadYDK(_deckName);
}

void StorageHandler::UseDefaultDeck() {
    for (byte i = 0; i < 35; i++) {
        DeckList[i] = _defaultDeckList[i];
    }
    IsDeckSet = true;
}

String StorageHandler::GetDeckName(int buttonEvents[]) {
    GetListOfDecks();

    for (int i = 0; i < 5; i++) {
        if (buttonEvents[i] == Enums::ButtonClicks::NoChange) continue;
        return _deckNames[i];
    }

    return String();
}

void StorageHandler::GetListOfDecks() {
    int i = 0;

    File dir = SD.open("/decks");
    while (true) {
        File entry = dir.openNextFile();
        if (!entry) break;

        String deckName = entry.name();
        _deckNames[i] = deckName.substring(7);
        i++;
    }

    String text[5];
    for (byte i = 0; i < 5; i++) {
        text[i] = String(i + 1) + ": " + _deckNames[i] + '\n';
    }
    _peripheralsHandler->Display(PeripheralsHandler::UI_DeckSelect, text);
}

// TODO: Handle Skill Cards
void StorageHandler::ReadYDK(String deckName) {
    _deckYDK = SD.open("/decks/" + deckName);

    if (!_deckYDK) {
        String text[] = { "Couldn't open file!" };
        _peripheralsHandler->Display(PeripheralsHandler::UI_DeckSelect, text);
        return;
    }

    int i = 0;
    String deckData[40];
    while (_deckYDK.available()) {        
        String data = _deckYDK.readStringUntil('\n');
        if (data.startsWith("#") || data.startsWith("!")) continue;
        
        deckData[i] = data;
        i++;
    }

    for (byte i = 0; i < 36; i++) {
        String filteredData = deckData[i].substring(0, 9);        
        char charArray[9];
        filteredData.toCharArray(charArray, 9);

        DeckList[i] = atoi(&charArray[0]);
    }

    IsDeckSet = true;
}