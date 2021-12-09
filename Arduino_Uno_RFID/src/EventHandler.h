#ifndef EventHandler_h
#define EventHandler_h
#include "Arduino.h"
#include "Core\Zone.h"
#include "Core\Entities\Enums.h"
#include "Core\Entities\YgoCard.h"

class EventHandler {
private:
	bool _debug;
	Enums::Events SetEventType(DualCardZone zone, Enums::SensorType sensor);

public:

	EventHandler(bool debug);

	String GetFormattedEventData(DualCardZone &zone, Enums::SensorType sensor);
};

#endif

