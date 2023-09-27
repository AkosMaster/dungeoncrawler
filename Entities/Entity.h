#ifndef ENTITY_H
#define ENTITY_H

typedef struct Entity Entity;

#include "../Items/Item.h"
#include "../Dungeon/DungeonLevel.h"

typedef enum EntityType {
	Invalid,
	Player,
	Object,
	DroppedItem,

	Enemy
} EntityType;

typedef void (*OnTurnFunction)(Entity* baseEntity);
typedef void (*DrawFunction)(Entity* baseEntity);
typedef void (*DeSpawnFunction)(Entity* baseEntity);

typedef struct Entity {
	//EntityType type;
	int health;
	char name[10];
	int speed; // what percentage of turns can the entity move in?

	int movementPoints;

	int y;
	int x;
	DungeonLevel* level;

	Item* inventory[10];

	// pointer to parent object, this is passed to our functions
	void* parentPtr;
	OnTurnFunction onTurn;
	DrawFunction draw;
	DeSpawnFunction deSpawn;
} Entity;

#endif