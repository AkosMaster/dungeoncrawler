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
#include "../Helpers/Drawing.h"

#define LevelMaxEntities 1000
typedef struct DungeonLevel {
	Tile tiles[LevelHeight][LevelWidth];
	Entity* entities[LevelMaxEntities];
	Entity* loadedEntities[LevelMaxEntities];

	int entityCount;
	int loadedEntityCount;

	Entity* currentPlayer;
	bool gameWon;

	int ratDeaths;
} DungeonLevel;

void DungeonLevel_AddEntity(DungeonLevel* level, Entity* entity);
void DungeonLevel_RemoveEntity(DungeonLevel* level, Entity* entity);

void DungeonLevel_DeSpawnAllEntities(DungeonLevel* level);

void DungeonLevel_FindLoadedEntities(DungeonLevel* level);
void DungeonLevel_OnTurnEntities(DungeonLevel* level);

// fill level with default tiles
void DungeonLevel_InitLevel(DungeonLevel* level);
void DungeonLevel_ClearLevel(DungeonLevel *level);

// draw level to screen
extern int ViewCenterY;
extern int ViewCenterX;

void DungeonLevel_DrawLevel(DungeonLevel *level);

// generate the whole level
void DungeonLevel_GenerateLevel(DungeonLevel *level);


void DungeonLevel_OpenCloseDoor(DungeonLevel* level, int y, int x);

#endif