#ifndef DUNGEONROOM_H
#define DUNGEONROOM_H

#include "DungeonTile.h"
#include "DungeonLevel.h"
#include "../Entities/Rat/ERat.h"
#include "../Entities/RatShrine/ERatShrine.h"
#include "../Entities/Crawler/ECrawler.h"
#include "../Entities/Chest/EChest.h"

typedef struct DungeonRoom {

	bool successfullyGenerated;

	int y;
	int x;
	int h;
	int w;

	DungeonLevel* level;

} DungeonRoom;

bool GenerateRoom(DungeonLevel *level, int y, int x, int maxheight, int maxwidth, DungeonRoom* roomInfo);
void ConnectRooms(DungeonLevel* level, DungeonRoom* roomA, DungeonRoom* roomB, bool specialPath);

#endif