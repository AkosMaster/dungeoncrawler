#ifndef DUNGEONLEVEL_H
#define DUNGEONLEVEL_H

#include "DungeonTile.h"

#define LevelHeight 200
#define LevelWidth 200

#define DrawnLevelHeight 30
#define DrawnLevelWidth 60

#define LoadedDistance 25

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
} DungeonLevel;

void AddEntityToLevel(DungeonLevel* level, Entity* entity);
void RemoveEntityFromLevel(DungeonLevel* level, Entity* entity);

void FindLoadedEntitiesInLevel(DungeonLevel* level);
void OnTurnEntitiesInLevel(DungeonLevel* level);

// fill level with default tiles
void ClearLevelEntities(DungeonLevel* level);
void ClearLevel(DungeonLevel *level);

// draw level to screen
extern int ViewCenterY;
extern int ViewCenterX;

void DrawLevel(DungeonLevel *level);

// generate the whole level
void GenerateLevel(DungeonLevel *level);

#endif