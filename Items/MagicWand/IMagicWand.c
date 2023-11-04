#include "../../debugmalloc.h"

#include "IMagicWand.h"
#include "../../Helpers/Drawing.h"
#include <ncurses/ncurses.h>

void IMagicWand_Interact_Attack(Item* baseItem, int dirY, int dirX) {
	IMagicWand* wand = (IMagicWand*)baseItem;

	DungeonLevel* level = baseItem->owner->level;
	int bulletY = baseItem->owner->y + dirY;
	int bulletX = baseItem->owner->x + dirX;

	// drawing setup
	wand->lastShotOriginY = bulletY;
	wand->lastShotOriginX = bulletX;
	wand->shotTimer = 12;
	wand->lastShotDeltaY = dirY;
	wand->lastShotDeltaX = dirX;
	wand->lastShotTravelLength = 0;

	while (level->tiles[bulletY][bulletX].walkable) {

		for (int i = 0; i < level->loadedEntityCount; i++) {
			Entity* entity = level->loadedEntities[i];
			if (entity->y == bulletY && entity->x == bulletX) {
				switch(wand->spellType) {
					case Spell_Freeze:
						entity->status_fx[FX_Frozen] = rand()%20 + 5;
						break;
				}
				return; // entity was hit, dont continue
			}
		}

		bulletY += dirY;
		bulletX += dirX;
		wand->lastShotTravelLength++;
	}
}

void IMagicWand_DrawEffects(Item* baseItem) {
	IMagicWand* wand = (IMagicWand*)baseItem;

	if (wand->shotTimer > 0) {
		wand->shotTimer--;

		int bulletY = wand->lastShotOriginY;
		int bulletX = wand->lastShotOriginX;

		// draw gun
		int screenY;
		int screenX;
		if (WorldToScreen(bulletY, bulletX, &screenY, &screenX)) {
			ColorPrintChar(screenY, screenX, '/', COLOR_MAGENTA, COLOR_BLACK);
		}
		
		int i = wand->lastShotTravelLength-1;
		bulletY += wand->lastShotDeltaY;
		bulletX += wand->lastShotDeltaX;

		// trace shot
		while (i > 0) {
			i--;
			
			if (WorldToScreen(bulletY, bulletX, &screenY, &screenX)) {
				ColorPrintChar(screenY, screenX, '*', COLOR_MAGENTA, COLOR_BLACK);
			}

			bulletY += wand->lastShotDeltaY;
			bulletX += wand->lastShotDeltaX;
		}
	}	
}

IMagicWand* Give_IMagicWand(Entity* owner, SpellType type) {
	IMagicWand* wand = (IMagicWand*)malloc(sizeof(IMagicWand));

	wand->baseItem = defaultItem;

	strcpy(wand->baseItem.name, "magic wand");
	wand->baseItem.symbol = '/';
	wand->baseItem.foreColor = COLOR_MAGENTA;
	wand->baseItem.backColor = COLOR_BLACK;

	wand->baseItem.owner = owner;
	wand->baseItem.drawEffects = IMagicWand_DrawEffects;

	wand->lastShotTravelLength = 0;
	wand->spellType = type;

	wand->baseItem.interact_Attack = IMagicWand_Interact_Attack;

	if (!Entity_AddItemToInventory(owner, &wand->baseItem)) {
		Item_Delete(&wand->baseItem);
		return 0;
	}

	return wand;
}