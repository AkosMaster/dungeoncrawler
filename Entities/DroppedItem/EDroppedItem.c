#include "EDroppedItem.h"
#include "../../Dungeon/DungeonLevel.h"

void EDroppedItem_Draw(Entity* baseEntity) {	
	static int blink_timer = 0;
	static bool blink = false;

	if (--blink_timer < 0) {
		blink = !blink;
		blink_timer = 20;
	}

	baseEntity->foreColor = blink ? COLOR_GREEN : COLOR_BLACK;
}

void EDroppedItem_Loot(Entity* baseEntity, Entity* looter) {

}

EDroppedItem* Spawn_EDroppedItem(DungeonLevel* level, int y, int x) {
	EDroppedItem* ditem = (EDroppedItem*)malloc(sizeof(EDroppedItem));

	ditem->baseEntity = defaultEntity;

	ditem->baseEntity.health = 10;
	ditem->baseEntity.speed = 0;
	ditem->baseEntity.team=DontAttack;
	ditem->baseEntity.symbol = '%';
	strcpy(ditem->baseEntity.name, "dropped item");
	ditem->baseEntity.foreColor = COLOR_GREEN;
	ditem->baseEntity.backColor = COLOR_BLACK;

	ditem->baseEntity.draw = EDroppedItem_Draw;
	ditem->baseEntity.interact_Loot = EDroppedItem_Loot;

	ditem->baseEntity.level = level;
	ditem->baseEntity.parentPtr = ditem;
	ditem->baseEntity.y = y;
	ditem->baseEntity.x = x;

	DungeonLevel_AddEntity(level, &ditem->baseEntity);

	return ditem;
}