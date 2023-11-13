#include "../debugmalloc.h"

#include "Entity.h"

void Entity_OnTurn(Entity* entity) {

	if (entity->status_fx[FX_Frozen]) {
		return;
	}

	if (entity->onTurn) {
		entity->onTurn(entity);
	}

	// progress on path
	if (NodeListLength(entity->currentPath) <= 1) {
		return;
	}

	Node newNode;
	entity->currentPath = PopNodeList(entity->currentPath, NodeListLength(entity->currentPath)-1, &newNode);

	int newY = newNode.y;
	int newX = newNode.x;

	if (entity->level->tiles[newY][newX].walkable) {
		entity->y = newY;
		entity->x = newX;
	}

	
}

void Entity_OnGameTick(Entity* entity) {
	// shorten status effects
	for (int i = 0; i < EntityStatusFXCount; i++) {
		if (entity->status_fx[i] > 0) {
			entity->status_fx[i]--;
		}
	}
}

void Entity_Draw(Entity* entity) {
	if (entity->draw) {
		entity->draw(entity);
	}

	int screenPosY;
	int screenPosX;

	if (WorldToScreen(entity->y, entity->x, &screenPosY, &screenPosX)) {
		ColorPrintChar(screenPosY, screenPosX, entity->symbol, entity->foreColor, entity->status_fx[FX_Frozen] ? COLOR_CYAN : entity->backColor);
	}
}

void Entity_deSpawn(Entity* entity) {
	if (entity->deSpawn) {
		entity->deSpawn(entity);
	}

	FreeNodeList(entity->currentPath);

	for (int i = 0; i < 10; i++) {
		if (entity->inventory[i]) {
			Item_Delete(entity->inventory[i]);
		}
	}

	DungeonLevel_RemoveEntity(entity->level, entity);
	free(entity); // frees parent object by default
}

void Entity_Damage(Entity* entity, Entity* attacker, int points) {
	if (entity->damage) {
		entity->damage(entity, attacker, points);
	}

	entity->health -= points;
	if (entity->health <= 0) {
		attacker->score += entity->score;
		Entity_deSpawn(entity);
	}
}

void Entity_Interact_Loot(Entity* entity, Entity* looter) {
	// rakjunk at minden targyat az egyik entitastol a masikhoz
	int lootCount = 0;
	for (int i = 0; i < 10; i++) {
		if (entity->inventory[i]) {
			Item* item = entity->inventory[i];
			if (Entity_AddItemToInventory(looter, item)) {
				Entity_RemoveItemFromInventory(entity, item);
				lootCount++;
			} else {
				WriteText("your inventory is full!");
				break;
			}
			
		}
	}

	// ha sikeres az interakcio hivjuk meg az entitas loot fuggvenyet is
	if (lootCount > 0 && entity->interact_Loot) {
		entity->interact_Loot(entity, looter);
	}
}

void Entity_SetDestination(Entity* entity, int y, int x) {
	ClearNodeList(entity->currentPath);
	entity->currentPath = Pathfind(entity->level, entity->y, entity->x, y, x, false, false);
}

void Entity_ClearDestination(Entity* entity) {
	ClearNodeList(entity->currentPath);
}

bool Entity_AddItemToInventory(Entity* entity, Item* item) {

	// ha stackelheto, es van mar ilyenunk, akkor adjuk hozza a meglevohoz
	if (item->stackable) {
		for (int i = 0; i < 10; i++) {
			if (entity->inventory[i]) {
				if (strcmp(entity->inventory[i]->name, item->name) == 0) {
					entity->inventory[i]->stackSize += item->stackSize;
					Item_Delete(item);
					return true;
				}
			}
		}
	}

	// kulonben rakjuk be egy ures helyre
	for (int i = 0; i < 10; i++) {
		if (!entity->inventory[i]) {
			entity->inventory[i] = item;
			item->owner = entity;
			return true;
		}
	}
	return false; // no free slots in inventory
}

void Entity_RemoveItemFromInventory(Entity* entity, Item* item) {
	for (int i = 0; i < 10; i++) {
		if (entity->inventory[i] == item) {
			entity->inventory[i] = 0;
			return;
		}
	}
}