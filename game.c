#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <ncurses/ncurses.h>
#include "Dungeon/DungeonLevel.h"
#include "Helpers/Pathfinding.h"

#include "Helpers/Drawing.h"

#include "Entities/Player/EPlayer.h"
#include "Entities/Crawler/ECrawler.h"
#include "Entities/Rat/ERat.h"

#include "Items/Gold/IGold.h"
#include "Items/Flintlock/IFlintlock.h"

#include <windows.h>

int main() {

	srand(time(0));

	initscr();
	start_color();
	nodelay(stdscr, TRUE);
	//raw();
	keypad(stdscr, TRUE);
	curs_set(0);

	printf("> Screen init.\n");

	WriteText("Welcome, adventurer!");

	DungeonLevel level0;
	
	DungeonLevel_ClearEntities(&level0);
	DungeonLevel_ClearLevel(&level0);
	DungeonLevel_GenerateLevel(&level0);
	
	printf("> Map generated.\n");

	EPlayer* player = Spawn_EPlayer(&level0, 13,13);
	
	player->baseEntity.inventory[0] = &Give_IGold(&player->baseEntity, 13)->baseItem;
	player->baseEntity.inventory[1] = &Give_IFlintlock(&player->baseEntity)->baseItem;

	printf("> Player spawned.\n");

	//for (int i = 0; i < 100; i++) {Spawn_ECrawler(&level0, 20,10);};
	
	int tick = 0;
	while(true) {
//		mvprintw(0, DrawnLevelWidth+2, "%d entities / %d thinking                 ", level0.entityCount, level0.loadedEntityCount);
		mvprintw(2, DrawnLevelWidth+2, "health: %d               ", player->baseEntity.health);
		DungeonLevel_FindLoadedEntities(&level0);
		//printf("	-FLE()\n");
		DungeonLevel_OnTurnEntities(&level0);
		//printf("	-OTE()\n");
		DungeonLevel_DrawLevel(&level0);
		//printf("	-DRW()\n");
		refresh();
		Sleep(SleepBeforeNextFrame); // if the player is really slow we dont wait for things to render so much
		//printf("> tick %d.\n", tick);
		tick++;
	}

	endwin(); // end curses mode
	return 0;
}