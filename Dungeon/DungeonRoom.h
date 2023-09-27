#ifndef DUNGEONROOM_H
#define DUNGEONROOM_H

#include "DungeonTile.h"
#include "DungeonLevel.h"
#include "../Entities/Rat/ERat.h"
#include "../Entities/Crawler/ECrawler.h"

typedef enum RoomType {
	Default,
} RoomType;

void GenerateRoom(DungeonLevel *level, int y, int x, int height, int width) {
	for (int i = y; i < y+height; i++) {
		for (int j = x; j < x + width; j++) {

			// these should be walls as it is the edge of the room
			if (i == y || i == y+height-1 || j == x || j == x+width-1) {
				level->tiles[i][j] = RoomWall;
			} else {
				level->tiles[i][j] = (i+j)%2 == 0 ? Floor0 : Floor2;
			}

		}
	}

	if (rand()%2 == 0)
		Spawn_ERat(level, y+1, x+1);

	if (rand()%2 == 0) {
		Spawn_ECrawler(level, y+1, x+1);
	}
}

#endif