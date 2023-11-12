#include "../debugmalloc.h"

#include "Item.h"

void Item_DrawEffects(Item* item) {
	if (item->drawEffects) {
		item->drawEffects(item);
	}
}

void Item_Delete(Item* item) {
	if (item->delete) {
		item->delete(item);
	}

	Entity_RemoveItemFromInventory(item->owner, item);
	free(item);
}

void Item_Interact_Reload(Item* item, Item* ammo) {
	if (item->interact_Reload) {
		item->interact_Reload(item, ammo);
	}
}

void Item_Interact_Attack(Item* item, int dirY, int dirX) {
	if (item->interact_Attack) {
		item->interact_Attack(item, dirY, dirX);
	}
}

void Item_Interact_Consume(Item* item) {
	if (item->interact_Consume) {
		item->interact_Consume(item);
	}
}

void ItemInteractMenu(Item* item) {
	if (item->interact_Reload) {
		WriteText("r) reload");
	}

	if (item->interact_Attack) {
		WriteText("a) attack");
	}

	if (item->interact_Consume) {
		WriteText("c) consume");
	}

	switch(WaitForInput(item->owner->level)) {
		case 'c':
			Item_Interact_Consume(item);
			break;
		case 'r':
			if (item->interact_Reload) {

				WriteText("choose ammo (0-9)");
				char slot = WaitForInput(item->owner->level)-'0';
				if (slot < 0 || slot > 9) {
					return;
				}
				if (!item->owner->inventory[(int)slot]) {
					return;
				}

				Item_Interact_Reload(item, item->owner->inventory[(int)slot]);
			}
			break;
		case 'a':
			if (item->interact_Attack) {
				WriteText("choose direction (wasd-eqxy)");
				char dir = WaitForInput(item->owner->level);
				int dirY = (dir == 'w' || dir == 'q' || dir == 'e' ? -1 : 0) + (dir == 's' || dir == 'y' || dir == 'x' ? 1 : 0);
				int dirX = (dir == 'd' || dir == 'e' || dir == 'x' ? 1 : 0) + (dir == 'a' || dir == 'q' || dir == 'y' ? -1 : 0);

				if (dirY != 0 || dirX != 0) {
					Item_Interact_Attack(item, dirY, dirX);
				}
			}
			break;
	}
}