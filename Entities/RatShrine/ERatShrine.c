#include "ERatShrine.h"
#include "../../Dungeon/DungeonLevel.h"

#include "../../Entities/DroppedItem/EDroppedItem.h"
#include "../../Items/Gold/IGold.h"
#include "../Rat/ERat.h"


void ERatShrine_Damage(Entity* baseEntity, Entity* attacker, int points) {
	baseEntity->level->ratDeaths += RAT_REVENGE_TRESHOLD;
	WriteText("The King Rat has been angered!");
}

void ERatShrine_Loot(Entity* baseEntity, Entity* looter) {
	baseEntity->level->ratDeaths += RAT_REVENGE_TRESHOLD;
	WriteText("The King Rat is furious!");
}

ERatShrine* Spawn_ERatShrine(DungeonLevel* level, int y, int x) {
	ERatShrine* shrine = (ERatShrine*)malloc(sizeof(ERatShrine));

	shrine->baseEntity = defaultEntity;

	shrine->baseEntity.health = 15;
	shrine->baseEntity.speed = 0;
	shrine->baseEntity.team=DontAttack;
	shrine->baseEntity.symbol = '{';
	strcpy(shrine->baseEntity.name, "shrine");
	shrine->baseEntity.foreColor = COLOR_YELLOW;
	shrine->baseEntity.backColor = COLOR_BLACK;

	shrine->baseEntity.interact_Loot = ERatShrine_Loot;
	shrine->baseEntity.damage = ERatShrine_Damage;

	shrine->baseEntity.level = level;
	shrine->baseEntity.parentPtr = shrine;
	shrine->baseEntity.y = y;
	shrine->baseEntity.x = x;

	DungeonLevel_AddEntity(level, &shrine->baseEntity);

	return shrine;
}