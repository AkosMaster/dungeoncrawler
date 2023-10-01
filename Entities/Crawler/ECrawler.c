#include "ECrawler.h"
#include "../../Dungeon/DungeonLevel.h"

void ECrawler_OnTurn(Entity* baseEntity) {
	ECrawler* crawler = baseEntity->parentPtr;

	if (crawler->biteCooldown > 0) {
		crawler->biteCooldown--;
		return;
	}

	DungeonLevel* level = baseEntity->level;

	int shortestDistance = 999999;
	for (int i = 0; i < level->loadedEntityCount; i++) {
		Entity* target = level->loadedEntities[i];

		if (target == baseEntity || target->team == Crawlers || target->team == DontAttack)
			continue;

		double distToEntity = abs(target->y-baseEntity->y) + abs(target->x-baseEntity->x);
		if (distToEntity >= shortestDistance)
			continue;

		bool canSeeTarget = CanSee(level, baseEntity->y, baseEntity->x, target->y, target->x);
		if (canSeeTarget) {
			Pathfind(level, &crawler->currentPath, baseEntity->y, baseEntity->x, target->y, target->x, false, false);
		}

		if (crawler->currentPath.length <= 4 && canSeeTarget) {

			// we are close to the player
			crawler->biteCooldown = 2;
			Entity_Damage(target, baseEntity, 10);
			return;
		}
	}

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

	baseEntity->backColor = crawler->biteCooldown > 0 ? COLOR_MAGENTA : COLOR_BLACK;
}

void ECrawler_Damage(Entity* baseEntity, Entity* attacker, int points) {
}

void ECrawler_DeSpawn(Entity* baseEntity) {
}

ECrawler* Spawn_ECrawler(DungeonLevel* level, int y, int x) {
	ECrawler* crawler = (ECrawler*)malloc(sizeof(ECrawler));

	static Entity ECrawler_BaseEntity = {
		.health=30, 
		.name="crawler", 
	 	.symbol = 'c',
	 	.foreColor = COLOR_RED,
	 	.backColor = COLOR_BLACK,
		.movementPoints=0,
		.speed=70, 
		.team = Crawlers,
		.damage=ECrawler_Damage, 
		.onTurn=ECrawler_OnTurn, 
		.draw=ECrawler_Draw, 
		.deSpawn=ECrawler_DeSpawn
	};

	crawler->baseEntity = ECrawler_BaseEntity;
	crawler->baseEntity.level = level;
	crawler->baseEntity.parentPtr = crawler;
	crawler->baseEntity.y = y;
	crawler->baseEntity.x = x;

	crawler->biteCooldown = 0;

	DungeonLevel_AddEntity(level, &crawler->baseEntity);

	return crawler;
}