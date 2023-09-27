#ifndef ERAT_H
#define ERAT_H

#include "../Entity.h"
#include "../../Helpers/Drawing.h"
#include "../../Dungeon/DungeonLevel.h"
#include <stdlib.h>

typedef struct ERat {

	Entity baseEntity;

} ERat;

ERat* Spawn_ERat(DungeonLevel* level, int y, int x);

#endif