#ifndef ERAT_H
#define ERAT_H

#define RAT_REVENGE_TRESHOLD 5

#include "../Entity.h"
#include "../../Helpers/Drawing.h"
#include "../../Dungeon/DungeonLevel.h"
#include <stdlib.h>

typedef struct ERat {

	Entity baseEntity;
	bool isRed;
	int biteCooldown;

} ERat;

ERat* Spawn_ERat(DungeonLevel* level, int y, int x, bool isRed);

#endif