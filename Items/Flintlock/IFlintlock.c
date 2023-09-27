
#include "IFlintlock.h"
#include "../../Helpers/Drawing.h"
#include <ncurses/ncurses.h>

void IFlintlock_OnUse(Item* baseItem) {
	IFlintlock* flintlock = (IFlintlock*)baseItem;

	if (!flintlock->loaded) {
		mvprintw(DrawnLevelHeight+2, 0, "flintlock> pick an item to load into the gun (0-9) ");
		char cmd = WaitForInput(baseItem->owner->level);

		if (cmd-'0' >= 0 && cmd-'0' < 10) {
			Item* item = baseItem->owner->inventory[cmd-'0'];

			if (!item) {
				mvprintw(DrawnLevelHeight+3, 0, "\tno item to load! ");
				return;
			}
			if (!item->stackable) {
				mvprintw(DrawnLevelHeight+3, 0, "\tcant be loaded!");
				return;
			}
			item->stackSize--;
			flintlock->loaded = true;
		}
	} else {
		mvprintw(DrawnLevelHeight+2, 0, "flintlock> direction to fire (wasd) ");
		char cmd = WaitForInput(baseItem->owner->level);

		int deltaY = 0;
		int deltaX = 0;

		switch(cmd) {
			case 'w':
				deltaY--;
				break;
			case 's':
				deltaY++;
				break;
			case 'a':
				deltaX--;
				break;
			case 'd':
				deltaX++;
				break;
			default:
				mvprintw(DrawnLevelHeight+3, 0, "\tmisfire!");
				baseItem->owner->health -= 15;
				flintlock->loaded = false;
				return;
				break;
		}

		flintlock->loaded = false;

		DungeonLevel* level = baseItem->owner->level;
		int bulletY = baseItem->owner->y + deltaY;
		int bulletX = baseItem->owner->x + deltaX;

		// drawing setup
		flintlock->lastShotOriginY = bulletY;
		flintlock->lastShotOriginX = bulletX;
		flintlock->shotTimer = 12;
		flintlock->lastShotDeltaY = deltaY;
		flintlock->lastShotDeltaX = deltaX;
		flintlock->lastShotTravelLength = 0;

		while (level->tiles[bulletY][bulletX].walkable) {
			bulletY += deltaY;
			bulletX += deltaX;
			flintlock->lastShotTravelLength++;
		}

	}
}

void IFlintlock_DrawEffects(Item* baseItem) {
	IFlintlock* flintlock = (IFlintlock*)baseItem;

	if (flintlock->shotTimer > 0) {
		flintlock->shotTimer--;

		int bulletY = flintlock->lastShotOriginY;
		int bulletX = flintlock->lastShotOriginX;

		// draw gun
		int screenY;
		int screenX;
		if (WorldToScreen(bulletY, bulletX, &screenY, &screenX)) {
			ColorPrintChar(screenY, screenX, 'r', COLOR_WHITE, COLOR_BLACK);
		}
		
		int i = flintlock->lastShotTravelLength-1;
		bulletY += flintlock->lastShotDeltaY;
		bulletX += flintlock->lastShotDeltaX;

		// trace shot
		while (i > 0) {
			i--;
			
			if (WorldToScreen(bulletY, bulletX, &screenY, &screenX)) {
				ColorPrintChar(screenY, screenX, 'o', COLOR_WHITE, COLOR_BLACK);
			}

			bulletY += flintlock->lastShotDeltaY;
			bulletX += flintlock->lastShotDeltaX;
		}
	}	
}

IFlintlock* Give_IFlintlock(Entity* owner) {
	IFlintlock* flintlock = (IFlintlock*)malloc(sizeof(IFlintlock));

	static Item IFlintlock_BaseItem = {.name="flintlock", .symbol='r', .foreColor=COLOR_BLACK, .backColor=COLOR_WHITE, .stackable=false, .stackSize=1};
	flintlock->baseItem = IFlintlock_BaseItem;

	flintlock->baseItem.owner = owner;
	flintlock->baseItem.onUse = IFlintlock_OnUse;
	flintlock->baseItem.drawEffects = IFlintlock_DrawEffects;

	return flintlock;
}