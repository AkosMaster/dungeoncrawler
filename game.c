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
#include "Items/MagicWand/IMagicWand.h"

#include <windows.h>	

#include "debugmalloc.h"

int main() {

	//debugmalloc_log_file("dmalloc.txt");

	srand(time(0));

	initscr();
	start_color();
	nodelay(stdscr, TRUE);
	keypad(stdscr, TRUE);
	curs_set(0);

	printf("> Screen init.\n");

	WriteText("Welcome, adventurer!");

	DungeonLevel level0;
	
	DungeonLevel_InitLevel(&level0);
	DungeonLevel_ClearLevel(&level0);
	DungeonLevel_GenerateLevel(&level0);
	
	printf("> Map generated.\n");

	EPlayer* player = Spawn_EPlayer(&level0, 13,13);
	
	Give_IGold(&player->baseEntity, 13);
	Give_IFlintlock(&player->baseEntity);
	//Give_IMagicWand(&player->baseEntity, Spell_Freeze);

	printf("> Player spawned.\n");
	
	int tick = 0;
	while(level0.currentPlayer) {
//		mvprintw(0, DrawnLevelWidth+2, "%d entities / %d thinking                 ", level0.entityCount, level0.loadedEntityCount);
		mvprintw(2, DrawnLevelWidth+2, "health: %d               ", player->baseEntity.health);
		DungeonLevel_FindLoadedEntities(&level0);
		//printf("	-FLE()\n");
		
		DungeonLevel_DrawLevel(&level0);
		//printf("	-DRW()\n");
		
		DungeonLevel_OnTurnEntities(&level0);
		//printf("	-OTE()\n");

		refresh();
		Sleep(SleepBeforeNextFrame);
		//printf("> tick %d.\n", tick);
		tick++;

		//debugmalloc_dump();
	}
	DungeonLevel_DeSpawnAllEntities(&level0);

	endwin(); // end curses mode
	return 0;
}