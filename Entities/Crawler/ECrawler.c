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
			Entity_SetDestination(baseEntity, target->y, target->x);
		}

		if (crawler->baseEntity.currentPath.length <= 4 && canSeeTarget) {
			// we are close to the player
			crawler->biteCooldown = 4;
			Entity_Damage(target, baseEntity, 10);
			return;
		}
	}
}

void ECrawler_Draw(Entity* baseEntity) {
	ECrawler* crawler = baseEntity->parentPtr;

	if (crawler->biteCooldown > 0) {
		for (int i = 0; i < crawler->baseEntity.currentPath.length; i++) {
			Node node = crawler->baseEntity.currentPath.items[i];
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

ECrawler* Spawn_ECrawler(DungeonLevel* level, int y, int x) {
	ECrawler* crawler = (ECrawler*)malloc(sizeof(ECrawler));

	crawler->baseEntity = defaultEntity;
	crawler->baseEntity.health = 30;
	crawler->baseEntity.speed = 70;
	crawler->baseEntity.team=Crawlers;
	crawler->baseEntity.symbol = 'c';
	strcpy(crawler->baseEntity.name, "crawler");
	crawler->baseEntity.foreColor = COLOR_RED;

	crawler->baseEntity.onTurn = ECrawler_OnTurn;
	crawler->baseEntity.draw = ECrawler_Draw;

	crawler->baseEntity.level = level;
	crawler->baseEntity.parentPtr = crawler;
	crawler->baseEntity.y = y;
	crawler->baseEntity.x = x;

	crawler->biteCooldown = 0;
	InitNodeList(&crawler->baseEntity.currentPath);

	DungeonLevel_AddEntity(level, &crawler->baseEntity);

	return crawler;
}