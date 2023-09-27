#ifndef DUNGEONTILE_H
#define DUNGEONTILE_H

#include <ncurses/ncurses.h>

// a level consists of tiles
/*typedef enum TileType {
	
	Floor,
	Wall,
	Door,

} TileType;*/

typedef struct Tile {
	int id;
	char symbol;
	short foreColor;
	short backColor;
	bool walkable;
} Tile;

// tile definitions
static Tile Floor0 = {.id=0, .symbol='.', .foreColor=COLOR_WHITE, .backColor=COLOR_BLACK, .walkable=true};
static Tile Floor1 = {.id=1, .symbol=',', .foreColor=COLOR_WHITE, .backColor=COLOR_BLACK, .walkable=true};
static Tile Floor2 = {.id=2, .symbol='_', .foreColor=COLOR_WHITE, .backColor=COLOR_BLACK, .walkable=true};
static Tile CaveWall = {.id=3, .symbol=' ', .foreColor=COLOR_BLACK, .backColor=COLOR_BLACK, .walkable=false};
static Tile RoomWall = {.id=3, .symbol=' ', .foreColor=COLOR_BLACK, .backColor=COLOR_WHITE, .walkable=false};

void DrawTile(int y, int x, Tile t);

#endif