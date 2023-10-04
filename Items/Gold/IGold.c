
#include "IGold.h"
#include <ncurses/ncurses.h>

void IGold_OnUse(Item* baseItem) {

}

IGold* Give_IGold(Entity* owner, int stackSize) {
	IGold* gold = (IGold*)malloc(sizeof(IGold));

	//static Item IGold_BaseItem = {.name="gold", .symbol='$', .foreColor=COLOR_BLACK, .backColor=COLOR_YELLOW, .stackable=true, .stackSize=0};
	gold->baseItem = defaultItem;
	
	gold->baseItem.owner = owner;
	
	strcpy(gold->baseItem.name, "gold");
	gold->baseItem.symbol = '$';
	gold->baseItem.foreColor = COLOR_BLACK;
	gold->baseItem.backColor = COLOR_YELLOW;
	gold->baseItem.stackable = true;

	gold->baseItem.stackSize = stackSize;
	gold->baseItem.drawEffects = 0;

	return gold;
}