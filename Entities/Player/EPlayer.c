#include "EPlayer.h"
#include "../../Dungeon/DungeonLevel.h"

#include <time.h>

void EPlayer_OnTurn(Entity* baseEntity) {
	EPlayer* player = baseEntity->parentPtr;
	DungeonLevel* level = baseEntity->level;

	mvprintw(DrawnLevelHeight+2, 0, "> \t\t\t\t\t\t\t\t");

	char command = WaitForInput(level);

	// process player movement
	int deltaY=0;
	int deltaX=0;
	switch(command) {
		case 'w':
			deltaY--;
			break;
		case 's':
			deltaY++;
			break;
		case 'a':
			deltaX--;
			break;
		case 'd':
			deltaX++;
			break;
		default:
			break;
	}

	int newY = baseEntity->y + deltaY;
	int newX = baseEntity->x + deltaX;
	if (level->tiles[newY][newX].walkable) {
		baseEntity->y = newY;
		baseEntity->x = newX;
	}

	ViewCenterY = baseEntity->y;
	ViewCenterX = baseEntity->x;

	// process inventory commands
	if (command - '0' >= 0 && command - '0' < 10) {
		Item* item = baseEntity->inventory[command-'0'];

		if (item) {
			item->onUse(item);
		}
	}
}

void DrawInventory(EPlayer* player) {
	mvprintw(5, DrawnLevelWidth+2, "Inventory:");
	for (int i = 0; i < 10; i++) {

		if (player->baseEntity.inventory[i] != 0) {
			Item* item = player->baseEntity.inventory[i];
			mvprintw(6+i, DrawnLevelWidth+2, "(%d)", i);
			ColorPrintChar(6+i, DrawnLevelWidth+6, item->symbol, item->foreColor, item->backColor);

			if (item->stackable) {
				mvprintw(6+i, DrawnLevelWidth+8, "%s [%d]", item->name, item->stackSize);
			} else {
				mvprintw(6+i, DrawnLevelWidth+8, "%s ", item->name);
			}
		} else {
			mvprintw(6+i, DrawnLevelWidth+2, "#");
		}
	}
}

void EPlayer_Draw(Entity* baseEntity) {
	EPlayer* player = baseEntity->parentPtr;
	
	int screenPosY;
	int screenPosX;

	static int blink_timer = 0;
	static bool blink = false;

	if (--blink_timer < 0) {
		blink = !blink;
		blink_timer = 20;
	}

	if (WorldToScreen(baseEntity->y, baseEntity->x, &screenPosY, &screenPosX)) {
		ColorPrintChar(screenPosY, screenPosX, '@', COLOR_CYAN, blink ? COLOR_WHITE : COLOR_BLACK);
	}

	DrawInventory(player);
}

void EPlayer_DeSpawn(Entity* baseEntity) {
	EPlayer* player = baseEntity->parentPtr;
	RemoveEntityFromLevel(baseEntity->level, baseEntity);
	free(player);

	printf("Player was despawned\n");
	exit(1);
}

EPlayer* Spawn_EPlayer(DungeonLevel* level, int y, int x) {
	EPlayer* player = (EPlayer*)malloc(sizeof(EPlayer));

	static Entity EPlayer_BaseEntity = {.health=100, .name="player", .y=0, .x=0, .movementPoints=0, .speed=50, .level=0,  .parentPtr=0, .onTurn=EPlayer_OnTurn, .draw=EPlayer_Draw, .deSpawn=EPlayer_DeSpawn};

	player->baseEntity = EPlayer_BaseEntity;
	player->baseEntity.level = level;
	player->baseEntity.parentPtr = player;
	player->baseEntity.y = y;
	player->baseEntity.x = x;

	AddEntityToLevel(level, &player->baseEntity);
	level->currentPlayer = &player->baseEntity;

	for (int i = 0; i < 10; i++) {
		player->baseEntity.inventory[i] = 0;
	}

	return player;
}