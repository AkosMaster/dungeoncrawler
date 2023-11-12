#ifndef IHEALINGPOTION_H
#define IHEALINGPOTION_H

#include <stdlib.h>
#include "../Item.h"

typedef struct IHealingPotion {
	Item baseItem;
} IHealingPotion;

IHealingPotion* Give_IHealingPotion(Entity* owner, int stackSize);

#endif