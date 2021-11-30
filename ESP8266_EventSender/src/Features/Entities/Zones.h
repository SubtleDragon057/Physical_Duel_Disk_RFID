#ifndef Zones_h
#define Zones_h
#include "Arduino.h"

class Zone {
protected:
	int _cardID;
	int _copyNumber;

public:
	String Name;
	
	Zone();
	Zone(String name);

	int CurrentCard() { return _cardID; }
	int CopyNumber() { return _copyNumber; }
	void UpdateCurrentCard(int cardID, int copyNum);
};

class SingleCardZone : public Zone {
public:
	SingleCardZone(String name);
};

class MultiCardZone : public Zone {
public:
	MultiCardZone(String name);
};

#endif