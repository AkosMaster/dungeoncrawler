#ifndef IFLINTLOCK_H
#define IFLINTLOCK_H

#include <stdlib.h>
#include "../Item.h"

typedef struct IFlintlock {
	Item baseItem;

	bool loaded;

	int lastShotOriginY;
	int lastShotOriginX;
	int lastShotDeltaY;
	int lastShotDeltaX;
	int shotTimer;
	int lastShotTravelLength;

} IFlintlock;

IFlintlock* Give_IFlintlock(Entity* owner);

#endif