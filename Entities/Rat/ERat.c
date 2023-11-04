#include "../../debugmalloc.h"

#include "ERat.h"
#include "../../Dungeon/DungeonLevel.h"

#include "../../Entities/DroppedItem/EDroppedItem.h"
#include "../../Items/Gold/IGold.h"

void ERat_OnTurn(Entity* baseEntity) {
	ERat* rat = (ERat*)baseEntity;
	DungeonLevel* level = baseEntity->level;

	if (level->ratDeaths >= RAT_REVENGE_TRESHOLD) {
		rat->isRed = true;
	}

	if (rat->biteCooldown > 0) {
		rat->biteCooldown--;
		return;
	}

	// random movement, allows diagonal so rats can "slip" through holes in walls
	int deltaY=rand()%3-1;
	int deltaX=rand()%3-1;

	if (rat->isRed) {
		int shortestDistance = 999999;
		for (int i = 0; i < level->loadedEntityCount; i++) {
			Entity* target = level->loadedEntities[i];

			if (target == baseEntity || target->team == Rats || target->team == DontAttack)
				continue;

			double distToEntity = abs(target->y-baseEntity->y) + abs(target->x-baseEntity->x);
			if (distToEntity >= shortestDistance)
				continue;

			bool canSeeTarget = CanSee(level, baseEntity->y, baseEntity->x, target->y, target->x);
			if (canSeeTarget || distToEntity <= 5) {
				if (canSeeTarget && distToEntity <= 2) {
					// DAMAGE
					Entity_Damage(target, baseEntity, 5);
					rat->biteCooldown = 3;
				} else {
					deltaY = target->y <= baseEntity->y ? -1 : 1;
					deltaX = target->x <= baseEntity->x ? -1 : 1;
				}
			}
		}
	}

	int newY = baseEntity->y + deltaY;
	int newX = baseEntity->x + deltaX;
	if (level->tiles[newY][newX].walkable) {
		baseEntity->y = newY;
		baseEntity->x = newX;
	}
}

void ERat_Draw(Entity* baseEntity) {
	ERat* rat = (ERat*)baseEntity;
	
	baseEntity->foreColor = rat->isRed ? COLOR_RED : COLOR_WHITE;
	baseEntity->backColor = rat->biteCooldown > 0 ? COLOR_MAGENTA : COLOR_BLACK;
}

void ERat_DeSpawn(Entity* baseEntity) {
	EDroppedItem* ditem = Spawn_EDroppedItem(baseEntity->level, baseEntity->y, baseEntity->x);
	Give_IGold(&ditem->baseEntity, rand()%3+1);

	baseEntity->level->ratDeaths += 1;

	if (baseEntity->level->ratDeaths == RAT_REVENGE_TRESHOLD) {
		//TODO spawn king rat
		WriteText("The King Rat seeks revenge!");
	}
}

ERat* Spawn_ERat(DungeonLevel* level, int y, int x, bool isRed) {
	ERat* rat = (ERat*)malloc(sizeof(ERat));

	rat->baseEntity = defaultEntity;
	rat->baseEntity.level = level;
	//rat->baseEntity.parentPtr = rat;
	rat->baseEntity.y = y;
	rat->baseEntity.x = x;

	rat->baseEntity.health = 2;
	rat->baseEntity.speed = 60;
	rat->baseEntity.team=Rats;
	rat->baseEntity.symbol = 'r';
	strcpy(rat->baseEntity.name, "rat");
	rat->baseEntity.foreColor = COLOR_WHITE;
	rat->baseEntity.backColor = COLOR_BLACK;

	rat->baseEntity.onTurn = ERat_OnTurn;
	rat->baseEntity.draw = ERat_Draw;
	rat->baseEntity.deSpawn = ERat_DeSpawn;

	rat->isRed = isRed;
	rat->biteCooldown = 0;

	DungeonLevel_AddEntity(level, &rat->baseEntity);

	return rat;
}