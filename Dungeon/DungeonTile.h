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
extern const Tile Floor0;
extern const Tile Floor1;
extern const Tile Floor2;
extern const Tile Floor3;
extern const Tile CaveWall;
extern const Tile RoomWall;
extern const Tile ClosedDoor;
extern const Tile OpenDoor;

void DrawTile(int y, int x, Tile t);

#endif