#ifndef IGOLD_H
#define IGOLD_H

#include <stdlib.h>
#include "../Item.h"

typedef struct IGold {
	Item baseItem;
} IGold;

IGold* Give_IGold(Entity* owner, int stackSize);

#endif