#include "../../debugmalloc.h"

#include "IFlintlock.h"
#include "../../Helpers/Drawing.h"
#include <ncurses/ncurses.h>

void IFlintlock_Interact_Attack(Item* baseItem, int dirY, int dirX) {
	IFlintlock* flintlock = (IFlintlock*)baseItem;

	flintlock->loaded = false;

	DungeonLevel* level = baseItem->owner->level;
	int bulletY = baseItem->owner->y + dirY;
	int bulletX = baseItem->owner->x + dirX;

	// drawing setup
	flintlock->lastShotOriginY = bulletY;
	flintlock->lastShotOriginX = bulletX;
	flintlock->shotTimer = 12;
	flintlock->lastShotDeltaY = dirY;
	flintlock->lastShotDeltaX = dirX;
	flintlock->lastShotTravelLength = 0;

	while (level->tiles[bulletY][bulletX].walkable) {

		for (int i = 0; i < level->loadedEntityCount; i++) {
			Entity* entity = level->loadedEntities[i];
			if (entity->y == bulletY && entity->x == bulletX) {
				Entity_Damage(entity, baseItem->owner, 15);
			}
		}

		bulletY += dirY;
		bulletX += dirX;
		flintlock->lastShotTravelLength++;
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
	flintlock->baseItem.interact_Attack = IFlintlock_Interact_Attack;

	if (!Entity_AddItemToInventory(owner, &flintlock->baseItem)) {
		Item_Delete(&flintlock->baseItem);
		return 0;
	}

	return flintlock;
}