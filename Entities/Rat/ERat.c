#include "ERat.h"
#include "../../Dungeon/DungeonLevel.h"

void ERat_OnTurn(Entity* baseEntity) {
	ERat* rat = baseEntity->parentPtr;
	DungeonLevel* level = baseEntity->level;

	// random movement, allows diagonal so rats can "slip" through holes in walls
	int deltaY=rand()%3-1;
	int deltaX=rand()%3-1;

	int newY = baseEntity->y + deltaY;
	int newX = baseEntity->x + deltaX;
	if (level->tiles[newY][newX].walkable) {
		baseEntity->y = newY;
		baseEntity->x = newX;
	}
}

void ERat_Draw(Entity* baseEntity) {
	ERat* rat = baseEntity->parentPtr;
	
	int screenPosY;
	int screenPosX;

	if (WorldToScreen(baseEntity->y, baseEntity->x, &screenPosY, &screenPosX)) {
		ColorPrintChar(screenPosY, screenPosX, 'r', COLOR_WHITE, COLOR_BLACK);
	}
}

void ERat_DeSpawn(Entity* baseEntity) {
	ERat* rat = baseEntity->parentPtr;
	RemoveEntityFromLevel(baseEntity->level, baseEntity);
	free(rat);
}

ERat* Spawn_ERat(DungeonLevel* level, int y, int x) {
	ERat* rat = (ERat*)malloc(sizeof(ERat));

	static Entity ERat_BaseEntity = {.health=2, .name="rat", .y=0, .x=0, .level=0, .movementPoints=0, .speed=30, .parentPtr=0, .onTurn=ERat_OnTurn, .draw=ERat_Draw, .deSpawn=ERat_DeSpawn};

	rat->baseEntity = ERat_BaseEntity;
	rat->baseEntity.level = level;
	rat->baseEntity.parentPtr = rat;
	rat->baseEntity.y = y;
	rat->baseEntity.x = x;
	rat->baseEntity.speed = 3;

	AddEntityToLevel(level, &rat->baseEntity);

	return rat;
}