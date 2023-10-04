#ifndef ECRAWLER_H
#define ECRAWLER_H

#include "../Entity.h"
#include "../../Helpers/Drawing.h"
#include "../../Dungeon/DungeonLevel.h"
#include "../../Helpers/Pathfinding.h"
#include <stdlib.h>

typedef struct ECrawler {

	Entity baseEntity;
	int biteCooldown;

} ECrawler;

ECrawler* Spawn_ECrawler(DungeonLevel* level, int y, int x);

#endif