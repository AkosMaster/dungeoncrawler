#include "../../debugmalloc.h"

#include "IHealingPotion.h"
#include <ncurses/ncurses.h>

void IHealingPotion_Interact_Consume(Item* baseItem) {
	baseItem->owner->health += rand()%20+2;

	baseItem->stackSize--;
	if (baseItem->stackSize == 0) {
		Item_Delete(baseItem);
	}
}

IHealingPotion* Give_IHealingPotion(Entity* owner, int stackSize) {
	IHealingPotion* potion = (IHealingPotion*)malloc(sizeof(IHealingPotion));

	//static Item IGold_BaseItem = {.name="gold", .symbol='$', .foreColor=COLOR_BLACK, .backColor=COLOR_YELLOW, .stackable=true, .stackSize=0};
	potion->baseItem = defaultItem;
	
	potion->baseItem.owner = owner;
	
	strcpy(potion->baseItem.name, "potion");
	potion->baseItem.symbol = '&';
	potion->baseItem.foreColor = COLOR_MAGENTA;
	potion->baseItem.backColor = COLOR_BLUE;
	potion->baseItem.stackable = true;

	potion->baseItem.stackSize = stackSize;
	potion->baseItem.drawEffects = 0;

	potion->baseItem.interact_Consume = IHealingPotion_Interact_Consume;

	if (!Entity_AddItemToInventory(owner, &potion->baseItem)) {
		Item_Delete(&potion->baseItem);
		return 0;
	}

	return potion;
}