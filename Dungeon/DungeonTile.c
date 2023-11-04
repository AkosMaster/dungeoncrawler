#include "../debugmalloc.h"
#include "DungeonTile.h"
#include "../Helpers/Drawing.h"

const Tile Floor0 = {.id=0, .symbol='.', .foreColor=COLOR_WHITE, .backColor=COLOR_BLACK, .walkable=true};
const Tile Floor1 = {.id=1, .symbol=',', .foreColor=COLOR_WHITE, .backColor=COLOR_BLACK, .walkable=true};
const Tile Floor2 = {.id=2, .symbol='_', .foreColor=COLOR_WHITE, .backColor=COLOR_BLACK, .walkable=true};
const Tile Floor3 = {.id=3, .symbol=' ', .foreColor=COLOR_WHITE, .backColor=COLOR_BLACK, .walkable=true};
const Tile CaveWall = {.id=4, .symbol=' ', .foreColor=COLOR_BLACK, .backColor=COLOR_BLACK, .walkable=false};
const Tile RoomWall = {.id=5, .symbol=' ', .foreColor=COLOR_BLACK, .backColor=COLOR_WHITE, .walkable=false};
const Tile ClosedDoor = {.id=6, .symbol='#', .foreColor=COLOR_BLACK, .backColor=COLOR_YELLOW, .walkable=false};
const Tile OpenDoor = {.id=7, .symbol='_', .foreColor=COLOR_YELLOW, .backColor=COLOR_BLACK, .walkable=true};

void DrawTile(int y, int x, Tile t) {
	ColorPrintChar(y,x,t.symbol, t.foreColor, t.backColor);
}