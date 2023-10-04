#ifndef ITEM_H
#define ITEM_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct Item Item;
#include "../Entities/Entity.h"

typedef void (*Interaction_Reload)(Item* baseItem, Item* ammo);
typedef void (*DrawEffectsFunction)(Item* baseItem);

typedef struct Item {

	char name[10];
	char symbol;
	short foreColor;
	short backColor;

	bool stackable;
	int stackSize; // only if stackable

	Entity* owner;
	
	DrawEffectsFunction drawEffects;

	//interactions
	Interaction_Reload interact_Reload;

} Item;

const static Item defaultItem = {
	.name="default",
	.symbol='?',
	.foreColor=0,
	.backColor=0,

	.stackable=false,
	.stackSize=1,

	.drawEffects=0,
	.interact_Reload=0,
};

void Item_DrawEffects(Item* item);

void Item_Interact_Reload(Item* item, Item* ammo);

#endif