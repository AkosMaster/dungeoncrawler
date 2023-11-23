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

void gameOverScreen(bool win, int score, char* name) {

	printf("\n\n----- GAME OVER -----\n\n");

	if (!win) {
		printf("You died! Better luck next time!\n");
	} else {
		printf("Congratulations! You escaped the labyrinth!\n");
	}

	printf("Your score: %d\n", score);

	FILE* scoreboardfile_read = fopen("scoreboard.txt", "r");
	char entryname[50];
	int entryscore;

	FILE* scoreboardfile_append = fopen("scoreboard_updated.txt", "a");

	printf("\n----- SCOREBOARD -----\n\n");
	int place = 1;
	bool placedBefore = false;
	while (fscanf(scoreboardfile_read, "%s %d", entryname, &entryscore) >= 2) {
		
		if (entryscore <= score && !placedBefore) {
			fprintf(scoreboardfile_append, "%s %d\n", name, score);
			printf("%d. %s : %d <--\n", place, name, score);
			place++;
			placedBefore = true;
		}
		printf("%d. %s : %d\n", place, entryname, entryscore);
		fprintf(scoreboardfile_append, "%s %d\n", entryname, entryscore);
		place++;
	}

	if (!placedBefore) {
		fprintf(scoreboardfile_append, "%s %d\n", name, score);
		printf("%d. %s : %d <--\n", place, name, score);
	}

	fclose(scoreboardfile_read);
	fclose(scoreboardfile_append);

	remove("scoreboard.txt");
	rename("scoreboard_updated.txt", "scoreboard.txt");

	printf("\n----- ***** -----\n");
}

int main() {

	char playerName[50];
	printf("What is your name? ");
	scanf("%s", playerName);

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

	printf("> Player spawned.\n");
	
	int score = 0;

	while(level0.currentPlayer && !level0.gameWon) {
		score = level0.currentPlayer->score;
		DungeonLevel_FindLoadedEntities(&level0);;
		
		DungeonLevel_DrawLevel(&level0);
		
		DungeonLevel_OnTurnEntities(&level0);

		refresh();
		Sleep(SleepBeforeNextFrame);
	}
	DungeonLevel_DeSpawnAllEntities(&level0);

	endwin(); // end curses mode
	gameOverScreen(level0.gameWon, score, playerName);
	
	return 0;
}