#ifndef ERATSHRINE_H
#define ERATSHRINE_H

#include "../Entity.h"
#include "../../Helpers/Drawing.h"
#include "../../Dungeon/DungeonLevel.h"
#include <stdlib.h>

typedef struct ERatShrine {

	Entity baseEntity;

} ERatShrine;

ERatShrine* Spawn_ERatShrine(DungeonLevel* level, int y, int x);

#endif