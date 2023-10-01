#ifndef EDROPPEDITEM_H
#define EDROPPEDITEM_H

#include "../Entity.h"
#include "../../Items/Item.h"
#include "../../Helpers/Drawing.h"
#include "../../Dungeon/DungeonLevel.h"
#include <stdlib.h>

typedef struct EDroppedItem {

	Entity baseEntity;

} EDroppedItem;

EDroppedItem* Spawn_EDroppedItem(DungeonLevel* level, int y, int x);

#endif