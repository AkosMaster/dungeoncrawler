
#include "IFlintlock.h"
#include "../../Helpers/Drawing.h"
#include <ncurses/ncurses.h>

void IFlintlock_OnUse(Item* baseItem) {
	IFlintlock* flintlock = (IFlintlock*)baseItem;

	if (!flintlock->loaded) {
		WriteText("pick an item to load into the gun (0-9) ");
		char cmd = WaitForInput(baseItem->owner->level);

		if (cmd-'0' >= 0 && cmd-'0' < 10) {
			Item* item = baseItem->owner->inventory[cmd-'0'];

			if (!item) {
				WriteText("no item to load!");
				return;
			}
			if (!item->stackable) {
				WriteText("item cant be loaded!");
				return;
			}
			item->stackSize--;
			flintlock->loaded = true;
		}
	} else {
		WriteText("pick direction to fire (wasd)");
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
				WriteText("misfire! You hurt yourself!");
				Entity_Damage(baseItem->owner, baseItem->owner, 5);
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

			for (int i = 0; i < level->loadedEntityCount; i++) {
				Entity* entity = level->loadedEntities[i];
				if (entity->y == bulletY && entity->x == bulletX) {
					Entity_Damage(entity, baseItem->owner, 15);
				}
			}

			bulletY += deltaY;
			bulletX += deltaX;
			flintlock->lastShotTravelLength++;
		}

	}
}

void IFlintlock_Interact_Reload(Item* baseItem, Item* ammo) {
	WriteText("reloaded");
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

	flintlock->baseItem = defaultItem;

	strcpy(flintlock->baseItem.name, "flintlock");
	flintlock->baseItem.symbol = 'r';
	flintlock->baseItem.foreColor = COLOR_BLACK;
	flintlock->baseItem.backColor = COLOR_WHITE;

	flintlock->baseItem.owner = owner;
	flintlock->baseItem.drawEffects = IFlintlock_DrawEffects;

	flintlock->lastShotTravelLength = 0;

	flintlock->baseItem.interact_Reload = IFlintlock_Interact_Reload;

	return flintlock;
}