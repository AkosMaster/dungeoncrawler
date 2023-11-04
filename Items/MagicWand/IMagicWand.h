#ifndef IMAGICWAND_H
#define IMAGICWAND_H

#include <stdlib.h>
#include "../Item.h"

typedef enum SpellType {
	Spell_Freeze
} SpellType;

typedef struct IMagicWand {
	Item baseItem;

	int lastShotOriginY;
	int lastShotOriginX;
	int lastShotDeltaY;
	int lastShotDeltaX;
	int shotTimer;
	int lastShotTravelLength;

	SpellType spellType;

} IMagicWand;

IMagicWand* Give_IMagicWand(Entity* owner, SpellType type);

#endif