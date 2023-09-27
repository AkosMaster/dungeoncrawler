
#include "IGold.h"
#include <ncurses/ncurses.h>

void IGold_OnUse(Item* baseItem) {

}

IGold* Give_IGold(Entity* owner, int stackSize) {
	IGold* gold = (IGold*)malloc(sizeof(IGold));

	static Item IGold_BaseItem = {.name="gold", .symbol='$', .foreColor=COLOR_BLACK, .backColor=COLOR_YELLOW, .stackable=true, .stackSize=0};
	gold->baseItem = IGold_BaseItem;
	
	gold->baseItem.owner = owner;
	gold->baseItem.onUse = IGold_OnUse;
	gold->baseItem.stackSize = stackSize;
	gold->baseItem.drawEffects = 0;

	return gold;
}