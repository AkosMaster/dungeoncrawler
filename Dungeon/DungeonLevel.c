#include "DungeonLevel.h"
#include "DungeonRoom.h"
#include "../Helpers/Pathfinding.h"
#include "../Helpers/Drawing.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ncurses/ncurses.h>

void DungeonLevel_ClearEntities(DungeonLevel* level) {
	level->entityCount = 0;
	level->ratDeaths = 0;
} 

void DungeonLevel_AddEntity(DungeonLevel* level, Entity* entity) {
	level->entities[level->entityCount] = entity;
	level->entityCount++;

	if (level->entityCount == LevelMaxEntities) {
		printf("Too many entities in level!\n");
		exit(1);
	}
}

void DungeonLevel_RemoveEntity(DungeonLevel* level, Entity* entity) {
	for (int i = 0; i < level->entityCount; i++) {
		if (level->entities[i] == entity) {

			memmove(&level->entities[i], &level->entities[i+1], (level->entityCount-i-1)*sizeof(Entity*));
			level->entityCount--;
			return;
		}
	}
	return;
}

void DungeonLevel_FindLoadedEntities(DungeonLevel* level) {
	level->loadedEntityCount = 0;
	for (int i = 0; i < level->entityCount; i++) {
		
		Entity* entity = level->entities[i];
		if (entity == level->currentPlayer || (abs(entity->y - ViewCenterY) <= LoadedDistance && abs(entity->x - ViewCenterX) <= LoadedDistance)) {
			level->loadedEntities[level->loadedEntityCount] = entity;
			level->loadedEntityCount++;
		}
		
	}
}

void DungeonLevel_OnTurnEntities(DungeonLevel* level) {
	for (int i = 0; i < level->loadedEntityCount; i++) {
		Entity* entity = level->loadedEntities[i];

		entity->movementPoints += entity->speed;

		if (entity->movementPoints >= 100) {
			Entity_OnTurn(entity);
			entity->movementPoints -= 100;
		}
	}
}

void DungeonLevel_DrawEntities(DungeonLevel* level) {
	for (int i = 0; i < level->entityCount; i++) {
		
		Entity* entity = level->entities[i];
		if (CanSee(level, level->currentPlayer->y, level->currentPlayer->x, entity->y, entity->x)) {
			Entity_Draw(entity);

			// draw effects for each item in inventory
			for (int j = 0; j < 10; j++) {
				Item* item = entity->inventory[j];
				if (item && item->drawEffects) {
					item->drawEffects(item);
				}
			}
		}
	}
}

// fill level with default tiles
void DungeonLevel_ClearLevel(DungeonLevel *level) {
	for (int y = 0; y < LevelHeight; y++) {
		for (int x = 0; x < LevelWidth; x++) {
			level->tiles[y][x] = CaveWall;
		}
	}
}

int ViewCenterY = 0;
int ViewCenterX = 0;
void DungeonLevel_DrawLevel(DungeonLevel *level) {
	for (int y = 0; y < DrawnLevelHeight; y++) {
		for (int x = 0; x < DrawnLevelWidth; x++) {
			int worldPosY;
			int worldPosX;
			ScreenToWorld(y, x, &worldPosY, &worldPosX);

			if (worldPosY >= 0 && worldPosY < LevelHeight && worldPosX >= 0 && worldPosX < LevelWidth) {
				DrawTile(y, x, level->tiles[worldPosY][worldPosX]);
			} else {
				ColorPrintChar(y, x, '?', COLOR_BLACK, COLOR_WHITE);
			}
		}
	}

	// draw border
	for (int y = 0; y <= DrawnLevelHeight; y++) {
		ColorPrintChar(y, DrawnLevelWidth, '$', COLOR_BLACK, COLOR_WHITE);
	}
	for (int x = 0; x < DrawnLevelWidth; x++) {
		ColorPrintChar(DrawnLevelHeight, x, '$', COLOR_BLACK, COLOR_WHITE);
	}

	DungeonLevel_DrawEntities(level);
}

void GenerateTunnel(DungeonLevel *level) {
	int startY = rand()%LevelHeight;
	int startX = rand()%LevelWidth;

	int endY = rand()%LevelHeight;
	int endX = rand()%LevelWidth;

	List tunnelPath;
	Pathfind(level, &tunnelPath, startY,startX,endY,endX, true, false);

	for (int i = tunnelPath.length-1; i >= 0; i--) {
		Node n = tunnelPath.items[i];

		level->tiles[n.y][n.x] = Floor0;
		if (n.y + 1 < LevelHeight && rand()%3 != 0)
			level->tiles[n.y+1][n.x] = Floor1;
		if (n.y - 1 > 0 && rand()%3 != 0)
			level->tiles[n.y-1][n.x] = Floor1;
		if (n.x + 1 < LevelWidth && rand()%3 != 0)
			level->tiles[n.y][n.x+1] = Floor1;
		if (n.x - 1 > 0 && rand()%3 != 0)
			level->tiles[n.y][n.x-1] = Floor1;
	}
}


void DungeonLevel_GenerateLevel(DungeonLevel *level) {
	
	DungeonRoom startingRoom;
	GenerateRoom(level, 10, 10, 50, 50, &startingRoom);

	DungeonRoom rooms[500] = {startingRoom};
	int roomCount = 1;

	for (int i = 0; i < roomCount; i++) {
		DungeonRoom currentRoom = rooms[i];

		DungeonRoom bottomRoom;
		if (GenerateRoom(level, currentRoom.y + currentRoom.h + rand()%5 + 1, currentRoom.x + rand()%5-5, 50, 50, &bottomRoom)) {
			ConnectRooms(level, &currentRoom, &bottomRoom);

			rooms[roomCount] = bottomRoom;
			roomCount++;
		}

		DungeonRoom rightRoom;
		if (GenerateRoom(level, currentRoom.y + rand()%5 -5, currentRoom.x + currentRoom.w + rand()%5 + 1, 50, 50, &rightRoom)) {
			ConnectRooms(level, &currentRoom, &rightRoom);
		
			rooms[roomCount] = rightRoom;
			roomCount++;
		}
	}

	for (int i = 0; i < 5; i++) {
		GenerateTunnel(level);
	}
}