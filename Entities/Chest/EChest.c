#include "../../debugmalloc.h"

#include "EChest.h"
#include "../../Dungeon/DungeonLevel.h"

#include "../../Items/Gold/IGold.h"
#include "../../Items/MagicWand/IMagicWand.h"

void EChest_Draw(Entity* baseEntity) {	
	
}

void EChest_Loot(Entity* baseEntity, Entity* looter) {
	baseEntity->foreColor = COLOR_WHITE;
}

void EChest_GenerateItems(EChest* chest) {
	Give_IGold(&chest->baseEntity, rand()%10+1);

	if (rand()%4 == 0) {
		Give_IMagicWand(&chest->baseEntity, Spell_Freeze);
	}
}

EChest* Spawn_EChest(DungeonLevel* level, int y, int x) {
	EChest* chest = (EChest*)malloc(sizeof(EChest));

	chest->baseEntity = defaultEntity;

	chest->baseEntity.health = 20;
	chest->baseEntity.speed = 0;
	chest->baseEntity.team=DontAttack;
	chest->baseEntity.symbol = '=';
	strcpy(chest->baseEntity.name, "chest");
	chest->baseEntity.foreColor = COLOR_YELLOW;
	chest->baseEntity.backColor = COLOR_BLACK;

	chest->baseEntity.draw = EChest_Draw;
	chest->baseEntity.interact_Loot = EChest_Loot;

	chest->baseEntity.level = level;
	//ditem->baseEntity.parentPtr = ditem;
	chest->baseEntity.y = y;
	chest->baseEntity.x = x;

	DungeonLevel_AddEntity(level, &chest->baseEntity);
	EChest_GenerateItems(chest);

	return chest;
}