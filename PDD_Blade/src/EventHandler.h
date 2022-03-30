#ifndef EventHandler_h
#define EventHandler_h
#include "Arduino.h"
#include "Core\Zone.h"
#include "Core\Entities\Enums.h"

class EventHandler {
private:
	Enums::Events SetEventType(DualCardZone zone, int sensor);

public:

	EventHandler();

	String GetFormattedEventData(DualCardZone &zone, Enums::SingleCardZoneType zoneType);
};

#endif

