#ifndef DUNGEONLEVEL_H
#define DUNGEONLEVEL_H

#include "DungeonTile.h"

#define LevelHeight 200
#define LevelWidth 200

#define DrawnLevelHeight 25
#define DrawnLevelWidth 60

#define LoadedDistance 35

// we need to forward declare since entity and dungeonlevel need eachother
typedef struct DungeonLevel DungeonLevel;
#include "../Entities/Entity.h"

#define LevelMaxEntities 5000
typedef struct DungeonLevel {
	Tile tiles[LevelHeight][LevelWidth];
	Entity* entities[LevelMaxEntities];
	Entity* loadedEntities[LevelMaxEntities];

	int entityCount;
	int loadedEntityCount;

	Entity* currentPlayer;

	int ratDeaths;
} DungeonLevel;

void AddEntityToLevel(DungeonLevel* level, Entity* entity);
void RemoveEntityFromLevel(DungeonLevel* level, Entity* entity);

void DungeonLevel_FindLoadedEntities(DungeonLevel* level);
void DungeonLevel_OnTurnEntities(DungeonLevel* level);

// fill level with default tiles
void DungeonLevel_ClearEntities(DungeonLevel* level);
void DungeonLevel_ClearLevel(DungeonLevel *level);

// draw level to screen
extern int ViewCenterY;
extern int ViewCenterX;

void DungeonLevel_DrawLevel(DungeonLevel *level);

// generate the whole level
void DungeonLevel_GenerateLevel(DungeonLevel *level);


static void DungeonLevel_OpenCloseDoor(DungeonLevel* level, int y, int x) {
	if (level->tiles[y][x].id == ClosedDoor.id) {
		level->tiles[y][x] = OpenDoor;
	} else if (level->tiles[y][x].id == OpenDoor.id) {
		level->tiles[y][x] = ClosedDoor;
	}
}

#endif