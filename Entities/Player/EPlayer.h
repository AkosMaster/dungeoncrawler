#ifndef EPLAYER_H
#define EPLAYER_H

#include "../Entity.h"
#include "../../Helpers/Drawing.h"
#include "../../Dungeon/DungeonLevel.h"
#include "../../Items/Item.h"
#include <stdlib.h>

typedef struct EPlayer {

	Entity baseEntity;

} EPlayer;

EPlayer* Spawn_EPlayer(DungeonLevel* level, int y, int x);

#endif