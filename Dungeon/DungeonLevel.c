#include "../debugmalloc.h"

#include "DungeonLevel.h"
#include "DungeonRoom.h"
#include "../Helpers/Pathfinding.h"
#include "../Helpers/Drawing.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ncurses/ncurses.h>

void DungeonLevel_InitLevel(DungeonLevel* level) {
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

void DungeonLevel_DeSpawnAllEntities(DungeonLevel* level) {
	while (level->entityCount > 0) {
		Entity_deSpawn(level->entities[0]);
	}
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

		Entity_OnGameTick(entity);
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

	NodeList tunnelPath;
	InitNodeList(&tunnelPath);
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
	FreeNodeList(&tunnelPath);
}


void DungeonLevel_GenerateLevel(DungeonLevel *level) {

	DungeonRoom firstRoom;
	GenerateRoom(level, 10, 10, 50, 50, &firstRoom);
	
	DungeonRoom rooms[500];
	rooms[0] = firstRoom;

	int roomCount = 1;

	for (int i = 0; i < roomCount; i++) {
		DungeonRoom currentRoom = rooms[i];

		DungeonRoom bottomRoom;
		int bottomY = currentRoom.y + currentRoom.h + rand()%5 + 1;
		int bottomX = currentRoom.x + rand()%5-5;
		if (GenerateRoom(level, bottomY, bottomX, 50, 50, &bottomRoom)) {
			ConnectRooms(level, &currentRoom, &bottomRoom, false);

			rooms[roomCount] = bottomRoom;
			roomCount++;
		} else if (rand()%2 == 0) {
			for (int j = 0; j < roomCount; j++) {
				if (i == j)
					continue;
				if (rooms[j].y <= bottomY && rooms[j].y + rooms[j].h > bottomY) {
					if (rooms[j].x <= bottomX && rooms[j].x + rooms[j].w > bottomX) {
						ConnectRooms(level, &currentRoom, &rooms[j], false);
						break;
					}
				}
			}
		}

		DungeonRoom rightRoom;
		int rightY = currentRoom.y + rand()%5 -5;
		int rightX = currentRoom.x + currentRoom.w + rand()%5 + 1;
		if (GenerateRoom(level, rightY, rightX, 50, 50, &rightRoom)) {
			ConnectRooms(level, &currentRoom, &rightRoom, false);

			rooms[roomCount] = rightRoom;
			roomCount++;
		} else if (rand()%3 == 0){
			for (int j = 0; j < roomCount; j++) {
				if (i == j)
					continue;
				if (rooms[j].y <= rightY && rooms[j].y + rooms[j].h > rightY) {
					if (rooms[j].x <= rightX && rooms[j].x + rooms[j].w > rightX) {
						ConnectRooms(level, &currentRoom, &rooms[j], false);
						break;
					}
				}
			}
		}
	}

	for (int i = 0; i < 5; i++) {
		GenerateTunnel(level);
	}
}

void DungeonLevel_OpenCloseDoor(DungeonLevel* level, int y, int x) {
	if (level->tiles[y][x].id == ClosedDoor.id) {
		level->tiles[y][x] = OpenDoor;
	} else if (level->tiles[y][x].id == OpenDoor.id) {
		level->tiles[y][x] = ClosedDoor;
	}
}