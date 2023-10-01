#include "ERatShrine.h"
#include "../../Dungeon/DungeonLevel.h"

#include "../../Entities/DroppedItem/EDroppedItem.h"
#include "../../Items/Gold/IGold.h"

#define RAT_REVENGE_TRESHOLD 5

void ERatShrine_OnTurn(Entity* baseEntity) {

}

void ERatShrine_Draw(Entity* baseEntity) {

}

void ERatShrine_Damage(Entity* baseEntity, Entity* attacker, int points) {
	baseEntity->level->ratDeaths += 10;
}

void ERatShrine_DeSpawn(Entity* baseEntity) {

}

ERatShrine* Spawn_ERatShrine(DungeonLevel* level, int y, int x) {
	ERatShrine* shrine = (ERatShrine*)malloc(sizeof(ERatShrine));

	static Entity ERatShrine_BaseEntity = {
	 .health=15,
	 .name="rat shrine", 
	 .symbol = '{',
	 .foreColor = COLOR_YELLOW,
	 .backColor = COLOR_BLACK,
	 .movementPoints=0, 
	 .speed=50, 
	 .team=DontAttack,
	 .damage=ERatShrine_Damage, 
	 .onTurn=ERatShrine_OnTurn, 
	 .draw=ERatShrine_Draw, 
	 .deSpawn=ERatShrine_DeSpawn};

	shrine->baseEntity = ERatShrine_BaseEntity;
	shrine->baseEntity.level = level;
	shrine->baseEntity.parentPtr = shrine;
	shrine->baseEntity.y = y;
	shrine->baseEntity.x = x;

	DungeonLevel_AddEntity(level, &shrine->baseEntity);

	return shrine;
}