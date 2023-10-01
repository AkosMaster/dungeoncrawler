#include "EPlayer.h"
#include "../../Dungeon/DungeonLevel.h"
#include <time.h>

#include "../Crawler/ECrawler.h"

void InteractMenu(Entity* playerEntity) {

	DungeonLevel* level = playerEntity->level;

	mvprintw(DrawnLevelHeight+2, 0, "> pick a direction to interact (wasd)\t\t\t\t\t\t\t\t");
	char direction = WaitForInput(level);


	int y= playerEntity->y + (direction=='w' ? -1 : 0) + (direction=='s' ? 1 : 0);
	int x= playerEntity->x + (direction=='a' ? -1 : 0) + (direction=='d' ? 1 : 0);
	
	int options = 0;
	bool doors = false;
	if (level->tiles[y][x].id == ClosedDoor.id || level->tiles[y][x].id == OpenDoor.id) {
		mvprintw(DrawnLevelHeight+2+options, 0, " (e) open/close door\t\t\t\t\t\t\t\t");
		options++;
		doors = true;
	}

	if (doors && options == 1) { // only one option and there is a door....
		DungeonLevel_OpenCloseDoor(level, y, x);
		return;
	}

	char sel = WaitForInput(level);

	if (sel == 'e') {
		DungeonLevel_OpenCloseDoor(level, y, x);	
	}

	if (sel == 'g') {
		if (WaitForInput(level) == 'o') {
			if (WaitForInput(level) == 'd') {
				mvprintw(DrawnLevelHeight+2, 0, "> c: spawn crawler, i: immortality\t\t\t\t\t\t\t\t");

				switch (WaitForInput(level)) {
					case 'c':
						Spawn_ECrawler(level, y, x);
						break;
					case 'i':
						playerEntity->health = 99999999;
						break;
				}
			}
		}
	}
}

void EPlayer_OnTurn(Entity* baseEntity) {
	EPlayer* player = baseEntity->parentPtr;
	DungeonLevel* level = baseEntity->level;

	mvprintw(DrawnLevelHeight+2, 0, "> \t\t\t\t\t\t\t\t");

	char command = WaitForInput(level);

	// process player movement
	int deltaY=(command=='w' ? -1 : 0) + (command=='s' ? 1 : 0);
	int deltaX=(command=='a' ? -1 : 0) + (command=='d' ? 1 : 0);
	if (command == 'e')
		InteractMenu(baseEntity);

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

	baseEntity->backColor = blink ? COLOR_WHITE : COLOR_BLACK;

	DrawInventory(player);
}

void EPlayer_Damage(Entity* baseEntity, Entity* attacker, int points) {
}

void EPlayer_DeSpawn(Entity* baseEntity) {
	printf("Player was despawned\n");
	exit(1);
}

EPlayer* Spawn_EPlayer(DungeonLevel* level, int y, int x) {
	EPlayer* player = (EPlayer*)malloc(sizeof(EPlayer));

	static Entity EPlayer_BaseEntity = {
		.health=100, 
		.name="player", 
		.symbol = '@',
		.foreColor = COLOR_CYAN,
		.backColor = COLOR_BLACK,
		.movementPoints=0, 
		.speed=50, 
		.team=Humanoids,
		.damage=EPlayer_Damage, 
		.onTurn=EPlayer_OnTurn, 
		.draw=EPlayer_Draw, 
		.deSpawn=EPlayer_DeSpawn
	};

	player->baseEntity = EPlayer_BaseEntity;
	player->baseEntity.level = level;
	player->baseEntity.parentPtr = player;
	player->baseEntity.y = y;
	player->baseEntity.x = x;

	DungeonLevel_AddEntity(level, &player->baseEntity);
	level->currentPlayer = &player->baseEntity;

	for (int i = 0; i < 10; i++) {
		player->baseEntity.inventory[i] = 0;
	}

	return player;
}