#include "EPlayer.h"
#include "../../Dungeon/DungeonLevel.h"
#include <time.h>

#include "../Crawler/ECrawler.h"

void InteractMenu(Entity* playerEntity) {

	DungeonLevel* level = playerEntity->level;

	WriteText("pick a direction to interact (wasd)");
	char direction = WaitForInput(level);


	int y= playerEntity->y + (direction=='w' ? -1 : 0) + (direction=='s' ? 1 : 0);
	int x= playerEntity->x + (direction=='a' ? -1 : 0) + (direction=='d' ? 1 : 0);
	
	int options = 0;
	bool doors = false;
	if (level->tiles[y][x].id == ClosedDoor.id || level->tiles[y][x].id == OpenDoor.id) {
		WriteText("(e) open/close door");
		options++;
		doors = true;
	}

	Entity* entityOptions[10] = {0};
	for (int i = 0; i < level->loadedEntityCount && options < 10; i++) {
		Entity* ent = level->loadedEntities[i];

		if (ent->y == y && ent->x == x) {
			entityOptions[options] = ent;
			char buffer[50];
			sprintf(buffer, "%d) %s", options, ent->name);
			WriteText(buffer);

			options++;
		}
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
				WriteText("Welcome to godmode!");
				WriteText("c: spawn crawler, i: immortality");

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

	if (sel-'0' >= 0 && sel-'0' < 10) {
		if (entityOptions[sel-'0']) {
			Entity* ent = entityOptions[sel-'0'];

			if (ent->interact_Loot) {
				WriteText("l) loot");
			}

			char i = WaitForInput(level);

			switch(i) {
				case 'l':
					Entity_Interact_Loot(ent, playerEntity);
					break;
			}
		}
	} 
}

void EPlayer_OnTurn(Entity* baseEntity) {
	EPlayer* player = baseEntity->parentPtr;
	DungeonLevel* level = baseEntity->level;

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
			//item->onUse(item);
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

	player->baseEntity = defaultEntity;
	player->baseEntity.level = level;
	player->baseEntity.parentPtr = player;
	player->baseEntity.y = y;
	player->baseEntity.x = x;

	player->baseEntity.health = 100;
	player->baseEntity.speed = 50;
	player->baseEntity.team=None;
	player->baseEntity.symbol = '@';
	strcpy(player->baseEntity.name, "player");
	player->baseEntity.foreColor = COLOR_CYAN;
	player->baseEntity.backColor = COLOR_BLACK;

	player->baseEntity.onTurn = EPlayer_OnTurn;
	player->baseEntity.draw = EPlayer_Draw;
	player->baseEntity.damage = EPlayer_Damage;
	player->baseEntity.deSpawn = EPlayer_DeSpawn;

	DungeonLevel_AddEntity(level, &player->baseEntity);
	level->currentPlayer = &player->baseEntity;

	for (int i = 0; i < 10; i++) {
		player->baseEntity.inventory[i] = 0;
	}

	return player;
}