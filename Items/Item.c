#include "Item.h"

void Item_DrawEffects(Item* item) {
	if (item->drawEffects) {
		item->drawEffects(item);
	}
}

void Item_Interact_Reload(Item* item, Item* ammo) {
	if (item->interact_Reload) {
		item->interact_Reload(item, ammo);
	}
}