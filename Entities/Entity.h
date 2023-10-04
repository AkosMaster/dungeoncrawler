#ifndef ENTITY_H
#define ENTITY_H

typedef struct Entity Entity;

#include "../Items/Item.h"
#include "../Dungeon/DungeonLevel.h"
#include "../Helpers/Pathfinding.h"

typedef enum EntityTeam {
	None,
	DontAttack,
	Rats,
	Crawlers,
	Humanoids,
} EntityTeam;

typedef void (*OnTurnFunction)(Entity* baseEntity);
typedef void (*DrawFunction)(Entity* baseEntity);
typedef void (*DeSpawnFunction)(Entity* baseEntity);
typedef void (*DamageFunction)(Entity* baseEntity, Entity* attacker, int points);

typedef void (*Interaction_Loot)(Entity* baseEntity, Entity* looter);

typedef struct Entity {
	//EntityType type;

	char symbol;
	short foreColor;
	short backColor;

	int health;
	char name[20];
	int speed; // what percentage of turns can the entity move in?

	EntityTeam team;

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
	DamageFunction damage;

	// possible interactions
	Interaction_Loot interact_Loot;

	// pathfinding
	NodeList currentPath;

} Entity;

const static Entity defaultEntity = {
		.health=50, 
		.name="default", 
	 	.symbol = '?',
	 	.foreColor = 0,
	 	.backColor = 0,
		.movementPoints=0,
		.speed=50, 
		.team = None,
		.damage=0, 
		.onTurn=0, 
		.draw=0, 
		.deSpawn=0,
		.interact_Loot=0,
};

void Entity_OnTurn(Entity* entity);

void Entity_Draw(Entity* entity);

void Entity_deSpawn(Entity* entity);

void Entity_Damage(Entity* entity, Entity* attacker, int points);

void Entity_Interact_Loot(Entity* entity, Entity* looter);

void Entity_SetDestination(Entity* entity, int y, int x);

void Entity_ClearDestination(Entity* entity);
#endif




