#include "EDroppedItem.h"
#include "../../Dungeon/DungeonLevel.h"

void EDroppedItem_OnTurn(Entity* baseEntity) {
	
}

void EDroppedItem_Draw(Entity* baseEntity) {
	EDroppedItem* ditem = baseEntity->parentPtr;
	
	int screenPosY;
	int screenPosX;

	static int blink_timer = 0;
	static bool blink = false;

	if (--blink_timer < 0) {
		blink = !blink;
		blink_timer = 20;
	}

	baseEntity->foreColor = blink ? COLOR_GREEN : COLOR_BLACK;
}

void EDroppedItem_Damage(Entity* baseEntity, Entity* attacker, int points) {
}

void EDroppedItem_DeSpawn(Entity* baseEntity) {
}

EDroppedItem* Spawn_EDroppedItem(DungeonLevel* level, int y, int x) {
	EDroppedItem* ditem = (EDroppedItem*)malloc(sizeof(EDroppedItem));

	static Entity EDroppedItem_BaseEntity = {
	 .health=5,
	 .name="dropped item",
	 .symbol = '%',
	 .foreColor = COLOR_WHITE,
	 .backColor = COLOR_BLACK,
	 .movementPoints=0, 
	 .speed=60, 
	 .team=DontAttack,
	 .damage=EDroppedItem_Damage, 
	 .onTurn=EDroppedItem_OnTurn, 
	 .draw=EDroppedItem_Draw, 
	 .deSpawn=EDroppedItem_DeSpawn};

	ditem->baseEntity = EDroppedItem_BaseEntity;
	ditem->baseEntity.level = level;
	ditem->baseEntity.parentPtr = ditem;
	ditem->baseEntity.y = y;
	ditem->baseEntity.x = x;

	DungeonLevel_AddEntity(level, &ditem->baseEntity);

	return ditem;
}