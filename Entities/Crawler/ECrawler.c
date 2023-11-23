#include "../../debugmalloc.h"

#include "ECrawler.h"
#include "../../Dungeon/DungeonLevel.h"

void ECrawler_OnTurn(Entity* baseEntity) {
	ECrawler* crawler = (ECrawler*)baseEntity;

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

		if (NodeListLength(crawler->baseEntity.currentPath) <= 4 && canSeeTarget) {
			// we are close to the player
			crawler->biteCooldown = 4;
			Entity_Damage(target, baseEntity, 10);
			return;
		}
	}
}

void ECrawler_Draw(Entity* baseEntity) {
	ECrawler* crawler = (ECrawler*)baseEntity;

	if (crawler->biteCooldown > 0) {
		if (crawler->baseEntity.currentPath) {

			NodeList* node = crawler->baseEntity.currentPath;
			while (node != NULL && node->next != NULL) { //skip last node as it is at feet of crawler
				//dbg path
				int screenY;
				int screenX;
				if (WorldToScreen(node->data.y, node->data.x, &screenY, &screenX)) {
					ColorPrintChar(screenY, screenX, '#', COLOR_MAGENTA, COLOR_BLACK);
				} 
				node = node->next;
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
	//crawler->baseEntity.parentPtr = crawler;
	crawler->baseEntity.y = y;
	crawler->baseEntity.x = x;

	crawler->baseEntity.score = 10;

	crawler->biteCooldown = 0;

	DungeonLevel_AddEntity(level, &crawler->baseEntity);

	return crawler;
}