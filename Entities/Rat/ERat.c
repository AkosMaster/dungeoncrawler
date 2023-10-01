#include "ERat.h"
#include "../../Dungeon/DungeonLevel.h"

#include "../../Entities/DroppedItem/EDroppedItem.h"
#include "../../Items/Gold/IGold.h"

#define RAT_REVENGE_TRESHOLD 5

void ERat_OnTurn(Entity* baseEntity) {
	ERat* rat = baseEntity->parentPtr;
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
	ERat* rat = baseEntity->parentPtr;
	
	baseEntity->foreColor = rat->isRed ? COLOR_RED : COLOR_WHITE;
	baseEntity->backColor = rat->biteCooldown > 0 ? COLOR_MAGENTA : COLOR_BLACK;
}

void ERat_Damage(Entity* baseEntity, Entity* attacker, int points) {
}

void ERat_DeSpawn(Entity* baseEntity) {
	EDroppedItem* ditem = Spawn_EDroppedItem(baseEntity->level, baseEntity->y, baseEntity->x);
	Give_IGold(&ditem->baseEntity, rand()%3+1);

	baseEntity->level->ratDeaths += 1;

	if (baseEntity->level->ratDeaths == RAT_REVENGE_TRESHOLD) {
		//TODO spawn king rat
	}
}

ERat* Spawn_ERat(DungeonLevel* level, int y, int x, bool isRed) {
	ERat* rat = (ERat*)malloc(sizeof(ERat));

	static Entity ERat_BaseEntity = {
	 .health=2,
	 .name="rat", 
	 .symbol = 'r',
	 .foreColor = COLOR_WHITE,
	 .backColor = COLOR_BLACK,
	 .movementPoints=0, 
	 .speed=60,
	 .team=Rats,
	 .damage=ERat_Damage, 
	 .onTurn=ERat_OnTurn, 
	 .draw=ERat_Draw,
	 .deSpawn=ERat_DeSpawn};

	rat->baseEntity = ERat_BaseEntity;
	rat->baseEntity.level = level;
	rat->baseEntity.parentPtr = rat;
	rat->baseEntity.y = y;
	rat->baseEntity.x = x;

	rat->isRed = isRed;
	rat->biteCooldown = 0;

	DungeonLevel_AddEntity(level, &rat->baseEntity);

	return rat;
}