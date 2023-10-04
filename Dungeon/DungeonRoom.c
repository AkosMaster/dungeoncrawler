
#include "DungeonRoom.h"

bool CanBuildRoom(DungeonLevel* level, int y, int x, int height, int width) {
	for (int i = y; i < y + height; i++) {
		for (int j = x; j < x + width; j++) {

			if (i < 0 || i >= LevelHeight || j < 0 || j >= LevelWidth)
				return false;

			if (level->tiles[i][j].id != CaveWall.id)
				return false;
		}
	}
	return true;
}

bool GenerateRoom(DungeonLevel *level, int y, int x, int maxheight, int maxwidth, DungeonRoom* roomInfo) {
	int height = rand()%15 + 5;
	int width = rand()%15 + 5;

	if (!CanBuildRoom(level, y, x, height, width)) {
		return false;
	}

	DungeonRoom r = {.y = y, .x = x, .h = height, .w = width, .level=level};

	*roomInfo = r;

	for (int i = y; i < y+height; i++) {
		for (int j = x; j < x + width; j++) {

			// these should be walls as it is the edge of the room
			if (i == y || i == y+height-1 || j == x || j == x+width-1) {
				level->tiles[i][j] = RoomWall;
			} else {
				level->tiles[i][j] = (i+j)%2 == 0 ? Floor3 : Floor0;
			}
		}
	}

	if (rand()%5 == 0) {
		Spawn_ERat(level, y+1, x+1, rand()%13==0);
	} else
	if (rand()%5 == 0) {
		Spawn_ECrawler(level, y+rand()%(height-2)+1, x+rand()%(width-2)+1);
	} else
	if (rand()%5 == 0) {
		Spawn_ERatShrine(level, y+rand()%(height-2)+1, x+rand()%(width-2)+1);
		for (int i = 0; i < 10; i++) {
			Spawn_ERat(level, y+rand()%(height-2)+1, x+rand()%(width-2)+1, rand()%13==0);
		}
	}

	return true;
}

bool GeneratePath(DungeonLevel *level, int startY, int startX, int endY, int endX, bool special) {
	NodeList tunnelPath;
	InitNodeList(&tunnelPath);
	Pathfind(level, &tunnelPath, startY,startX,endY,endX, true, true);
	if (tunnelPath.length == 0) {
		FreeNodeList(&tunnelPath);
		return false;
	}

	for (int i = tunnelPath.length-1; i >= 0; i--) {
		Node n = tunnelPath.items[i];

		if (level->tiles[n.y][n.x].id == CaveWall.id) {
			level->tiles[n.y][n.x] = special ? Floor2 : Floor0;
		}
	}

	FreeNodeList(&tunnelPath);
	return true;
}

void ConnectRooms(DungeonLevel* level, DungeonRoom* roomA, DungeonRoom* roomB, bool specialPath) {
	DungeonRoom* leftRoom = roomA->x <= roomB->x ? roomA : roomB;
	DungeonRoom* rightRoom = roomA->x <= roomB->x ? roomB : roomA;

	int leftDoorY = 0;
	int leftDoorX = 0;

	int rightDoorY = 0;
	int rightDoorX = 0;
	if (leftRoom->y <= rightRoom->y) { // left room is lower Y than right room
		if (rand()%2 == 0) { // door is on bottom side of leftroom
			leftDoorY = leftRoom->y + leftRoom->h - 1;
			leftDoorX = leftRoom->x + rand()%(leftRoom->w-2) + 1;
		} else { // door is at right side of leftroom
			leftDoorY = leftRoom->y + rand()%(leftRoom->h-2) + 1;
			leftDoorX = leftRoom->x + leftRoom->w - 1;
		}

		if (rand()%2 == 0) { // top side of rightroom
			rightDoorY = rightRoom->y;
			rightDoorX = rightRoom->x + rand()%(rightRoom->w-2) + 1;
		} else { // left side of rightroom
			rightDoorY = rightRoom->y + rand()%(rightRoom->h-2) + 1;
			rightDoorX = rightRoom->x;
		}
	} else { // leftroom is higher y than rightroom
		if (rand()%2 == 0) { // topside of leftroom
			leftDoorY = leftRoom->y;
			leftDoorX = leftRoom->x + rand()%(leftRoom->w-2) + 1;
		} else { // rightside of leftroom
			leftDoorY = leftRoom->y + rand()%(leftRoom->h-2) + 1;
			leftDoorX = leftRoom->x + leftRoom->w - 1;	
		}

		if (rand()%2 == 0) { // bottom side of rightroom
			rightDoorY = rightRoom->y + rightRoom->h - 1;
			rightDoorX = rightRoom->x + rand()%(rightRoom->w-2) + 1;
		} else { // left side of rightroom
			rightDoorY = rightRoom->y + rand()%(rightRoom->h-2) + 1;
			rightDoorX = rightRoom->x;
		}
	}

	level->tiles[leftDoorY][leftDoorX] = rand()%10==0 ? OpenDoor : ClosedDoor;
	level->tiles[rightDoorY][rightDoorX] = rand()%10==0 ? OpenDoor : ClosedDoor;

	GeneratePath(level, leftDoorY, leftDoorX, rightDoorY, rightDoorX, specialPath);
}













