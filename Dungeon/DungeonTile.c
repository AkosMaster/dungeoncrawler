#include "DungeonTile.h"
#include "../Helpers/Drawing.h"

void DrawTile(int y, int x, Tile t) {
	ColorPrintChar(y,x,t.symbol, t.foreColor, t.backColor);
}