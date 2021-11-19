#ifndef EventHandler_h
#define EventHandler_h
#include "Arduino.h"
#include "Core\Zone.h"
#include "Core\Entities\Enums.h"

class EventHandler {
public:

	EventHandler();

	String FormatEventInfo(DualCardZone zone, Enums::Events eventType, Enums::SensorType sensor);
	Enums::Events SetEventType(DualCardZone zone, Enums::SensorType sensor);
};

#endif

