#include "StorageHandler.h"
#include "SD.h"
#include "Core\Entities\Enums.h"

StorageHandler::StorageHandler()
{
}

void StorageHandler::ChooseDeck(int buttonEvents[]) {
    _deckName = GetDeckName(buttonEvents);
    if (_deckName == String()) return;
    
    ReadYDK(_deckName);
}

String StorageHandler::GetDeckName(int buttonEvents[]) {
    if (_deckNames->isEmpty()) {
        GetListOfDecks();
    }

    for (int i = 0; i < 5; i++) {
        if (buttonEvents[i] == Enums::ButtonClicks::NoChange) continue;

        Serial.printf("You chose: %i, %s\n", (i + 1), _deckNames[i].c_str());
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

    Serial.printf("Your deck options are:\n");
    for (byte i = 0; i < _deckNames->length() - 1; i++) {
        Serial.printf("%i: %s\n", (i + 1), _deckNames[i].c_str());
    }
    Serial.printf("\nPlease Choose Your Deck Using The Buttons!\n");
}

void StorageHandler::ReadYDK(String deckName) {
    _deckYDK = SD.open("/decks/" + deckName);

    if (!_deckYDK) {
        Serial.printf("Couldn't open file!\n");
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