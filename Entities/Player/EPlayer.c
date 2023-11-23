#include "../../debugmalloc.h"

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
	bool door = false;
	if (level->tiles[y][x].id == ClosedDoor.id || level->tiles[y][x].id == OpenDoor.id) {
		options++;
		door = true;
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

	if (door && options == 1) { // only one option and there is a door....
		DungeonLevel_OpenCloseDoor(level, y, x);
		return;
	}

	if (door)
		WriteText("e) open/close door");

	char sel = WaitForInput(level);

	if (sel == 'e') {
		DungeonLevel_OpenCloseDoor(level, y, x);	
	} 
	else if (sel == 'g') {
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
	else if (sel-'0' >= 0 && sel-'0' < 10) {
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

		if (level->tiles[newY][newX].id == 8) {
			// player wins game

			level->gameWon = true;
		}
	}

	ViewCenterY = baseEntity->y;
	ViewCenterX = baseEntity->x;

	// process inventory commands
	if (command - '0' >= 0 && command - '0' < 10) {
		Item* item = baseEntity->inventory[command-'0'];

		if (item) {
			ItemInteractMenu(item);
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
			mvprintw(6+i, DrawnLevelWidth+2, "#                  ");
		}
	}
}

void EPlayer_Draw(Entity* baseEntity) {
	EPlayer* player = (EPlayer*)baseEntity;

	static int blink_timer = 0;
	static bool blink = false;

	if (--blink_timer < 0) {
		blink = !blink;
		blink_timer = 20;
	}

	baseEntity->backColor = blink ? COLOR_WHITE : COLOR_BLACK;

	mvprintw(2, DrawnLevelWidth+2, "health: %d               ", player->baseEntity.health);
	mvprintw(1, DrawnLevelWidth+2, "score: %d               ", player->baseEntity.score);
	DrawInventory(player);
}

void EPlayer_Damage(Entity* baseEntity, Entity* attacker, int points) {
}

void EPlayer_DeSpawn(Entity* baseEntity) {
	printf("Player was despawned\n");
	baseEntity->level->currentPlayer = 0;
}

EPlayer* Spawn_EPlayer(DungeonLevel* level, int y, int x) {
	EPlayer* player = (EPlayer*)malloc(sizeof(EPlayer));

	player->baseEntity = defaultEntity;
	player->baseEntity.level = level;
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