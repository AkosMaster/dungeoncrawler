#include "../../debugmalloc.h"

#include "IHealingPotion.h"
#include <ncurses/ncurses.h>

void IHealingPotion_Interact_Consume(Item* baseItem) {
	baseItem->owner.health += rand()%20+2;
}

IHealingPotion* Give_IHealingPotion(Entity* owner, int stackSize) {
	IHealingPotion* potion = (IHealingPotion*)malloc(sizeof(IHealingPotion));

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

	if (!Entity_AddItemToInventory(owner, &gold->baseItem)) {
		Item_Delete(&gold->baseItem);
		return 0;
	}

	return gold;
}