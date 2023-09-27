#ifndef DRAWING_H
#define DRAWING_H

#include <stdbool.h>
#include "../Dungeon/DungeonLevel.h"

#define SleepBeforeNextFrame 20

// w2s returns false if the screen position is out of bounds
bool WorldToScreen(int y, int x, int *sy, int *sx);
void ScreenToWolrd(int y, int x, int *wy, int *wx);

void ColorPrintChar(int y, int x, char c, short fore, short back);

bool CanSee(DungeonLevel* level, int fromY, int fromX, int toY, int toX);

// waits for an input key and displays the level that the input is being asked for from
char WaitForInput(DungeonLevel* level);

#endif