#ifndef ITEM_H
#define ITEM_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct Item Item;
#include "../Entities/Entity.h"

typedef void (*OnUseFunction)(Item* baseItem);
typedef void (*DrawEffectsFunction)(Item* baseItem);

typedef struct Item {

	char name[10];
	char symbol;
	short foreColor;
	short backColor;

	bool stackable;
	int stackSize; // only if stackable

	Entity* owner;
	
	OnUseFunction onUse;
	DrawEffectsFunction drawEffects;

} Item;

#endif