#include "ECrawler.h"
#include "../../Dungeon/DungeonLevel.h"

void ECrawler_OnTurn(Entity* baseEntity) {
	ECrawler* crawler = baseEntity->parentPtr;

	if (crawler->biteCooldown > 0) {
		crawler->biteCooldown--;
		return;
	}

	DungeonLevel* level = baseEntity->level;

	bool canSeePlayer = CanSee(level, baseEntity->y, baseEntity->x, level->currentPlayer->y, level->currentPlayer->x);
	if (canSeePlayer) {
		Pathfind(level, &crawler->currentPath, baseEntity->y, baseEntity->x, level->currentPlayer->y, level->currentPlayer->x, false);
	}

	if (crawler->currentPath.length <= 4 && canSeePlayer) {

		// we are close to the player
		crawler->biteCooldown = 2;
		level->currentPlayer->health -= 10;
	}

	// debug pathfinding
	/*for (int i = 0; i < crawler->currentPath.length; i++) {
		Node node = crawler->currentPath.items[i];
		//dbg path
		int screenY;
		int screenX;
		if (WorldToScreen(node.y, node.x, &screenY, &screenX)) {
			ColorPrintChar(screenY, screenX, '#', COLOR_MAGENTA, COLOR_YELLOW);
		}
	}*/

	// process crawler movement

	if (crawler->currentPath.length <= 1)
		return;
	int newY = crawler->currentPath.items[crawler->currentPath.length-2].y;
	int newX = crawler->currentPath.items[crawler->currentPath.length-2].x;
	if (level->tiles[newY][newX].walkable) {
		baseEntity->y = newY;
		baseEntity->x = newX;
	}
	crawler->currentPath.length--;
}

void ECrawler_Draw(Entity* baseEntity) {
	ECrawler* crawler = baseEntity->parentPtr;
	
	int screenPosY;
	int screenPosX;

	if (crawler->biteCooldown > 0) {
		for (int i = 0; i < crawler->currentPath.length; i++) {
			Node node = crawler->currentPath.items[i];
			//dbg path
			int screenY;
			int screenX;
			if (WorldToScreen(node.y, node.x, &screenY, &screenX)) {
				ColorPrintChar(screenY, screenX, '#', COLOR_MAGENTA, COLOR_BLACK);
			}
		} 
	}

	if (WorldToScreen(baseEntity->y, baseEntity->x, &screenPosY, &screenPosX)) {
		ColorPrintChar(screenPosY, screenPosX, 'c', COLOR_RED, crawler->biteCooldown > 0 ? COLOR_MAGENTA : COLOR_BLACK);
	}
}

void ECrawler_DeSpawn(Entity* baseEntity) {
	ECrawler* crawler = baseEntity->parentPtr;
	RemoveEntityFromLevel(baseEntity->level, baseEntity);
	free(crawler);
}

ECrawler* Spawn_ECrawler(DungeonLevel* level, int y, int x) {
	ECrawler* crawler = (ECrawler*)malloc(sizeof(ECrawler));

	static Entity ECrawler_BaseEntity = {.health=30, .name="crawler", .y=0, .x=0, .level=0, .movementPoints=0, .speed=70, .parentPtr=0, .onTurn=ECrawler_OnTurn, .draw=ECrawler_Draw, .deSpawn=ECrawler_DeSpawn};

	crawler->baseEntity = ECrawler_BaseEntity;
	crawler->baseEntity.level = level;
	crawler->baseEntity.parentPtr = crawler;
	crawler->baseEntity.y = y;
	crawler->baseEntity.x = x;

	AddEntityToLevel(level, &crawler->baseEntity);

	return crawler;
}