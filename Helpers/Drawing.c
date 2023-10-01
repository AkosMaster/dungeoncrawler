#include "Drawing.h"
#include <ncurses/ncurses.h>
#include "../Dungeon/DungeonLevel.h"
#include <math.h>
#include <windows.h>

void ColorPrintChar(int y, int x, char c, short fore, short back) {
	
	// fore and back can each go from 0 to 7
	int pairnum = 8*back + fore;

	init_pair(pairnum, fore, back);

	attron(COLOR_PAIR(pairnum));
	mvaddch(y,x,c);
	attroff(COLOR_PAIR(pairnum));
}

bool WorldToScreen(int y, int x, int *sy, int *sx) {
	*sy = y - ViewCenterY + DrawnLevelHeight/2;
	*sx = x - ViewCenterX + DrawnLevelWidth/2;

	if (*sy < 0 || *sy >= DrawnLevelHeight)
		return false;
	if (*sx < 0 || *sx >= DrawnLevelWidth)
		return false;

	return true;
}

void ScreenToWorld(int y, int x, int *wy, int *wx) {
	*wy = y + ViewCenterY - DrawnLevelHeight/2;
	*wx = x + ViewCenterX - DrawnLevelWidth/2;
}


bool CanSee(DungeonLevel* level, int fromY, int fromX, int toY, int toX) {

	// based on https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm

	int dx = abs(toX - fromX);
	int sx = fromX < toX ? 1 : -1;
	int dy = abs(toY - fromY);
	int sy = fromY < toY ? 1 : -1;
	int error = (dx > dy ? dx : -dy) / 2;
	int error2;

  	while (true) {
    	// setPixel(fromX,fromY,Matrix);

  		if (!level->tiles[fromY][fromX].walkable) {
  			return false;
  		}

    	/*int screenY;
    	int screenX;
    	if (WorldToScreen(fromY, fromX, &screenY, &screenX)) {
    		ColorPrintChar(screenY, screenX, '@', COLOR_MAGENTA, COLOR_GREEN);
    	}*/

    	if (fromX == toX && fromY == toY)
      		break;
    	error2 = error;
    	if (error2 > -dx) {
      		error -= dy;
      		fromX += sx;
    	}
    	if (error2 < dy) {
      		error += dx;
      		fromY += sy;
    	}
  	}
  	return true;
}

char WaitForInput(DungeonLevel* level) {
	
	char ch = getch();
	while (ch == ERR) {
		DungeonLevel_DrawLevel(level);
		Sleep(SleepBeforeNextFrame);

		ch = getch();
	}
	return ch;
}