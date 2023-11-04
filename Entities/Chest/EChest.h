#ifndef ECHEST_H
#define ECHEST_H

#include "../Entity.h"
#include "../../Items/Item.h"
#include "../../Helpers/Drawing.h"
#include "../../Dungeon/DungeonLevel.h"
#include <stdlib.h>

typedef struct EChest {

	Entity baseEntity;

} EChest;

EChest* Spawn_EChest(DungeonLevel* level, int y, int x);

#endif