#include "Entity.h"

void Entity_OnTurn(Entity* entity) {
	if (entity->onTurn) {
		entity->onTurn(entity);
	}

	// progress on path
	if (entity->currentPath.length <= 1) {
		return;
	}

	int newY = entity->currentPath.items[entity->currentPath.length-2].y;
	int newX = entity->currentPath.items[entity->currentPath.length-2].x;

	if (entity->level->tiles[newY][newX].walkable) {
		entity->y = newY;
		entity->x = newX;
	}
	//entity->currentPath.length--;
	PopNodeList(&entity->currentPath, entity->currentPath.length-1);
}

void Entity_Draw(Entity* entity) {
	if (entity->draw) {
		entity->draw(entity);
	}

	int screenPosY;
	int screenPosX;

	if (WorldToScreen(entity->y, entity->x, &screenPosY, &screenPosX)) {
		ColorPrintChar(screenPosY, screenPosX, entity->symbol, entity->foreColor, entity->backColor);
	}
}

void Entity_deSpawn(Entity* entity) {
	if (entity->deSpawn) {
		entity->deSpawn(entity);
	}

	if (IsNodeListInit(&entity->currentPath)) {
		FreeNodeList(&entity->currentPath);
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
		Entity_deSpawn(entity);
	}
}

void Entity_Interact_Loot(Entity* entity, Entity* looter) {
	if (entity->interact_Loot) {
		entity->interact_Loot(entity, looter);
	}
}

void Entity_SetDestination(Entity* entity, int y, int x) {
	ClearNodeList(&entity->currentPath);
	Pathfind(entity->level, &entity->currentPath, entity->y, entity->x, y, x, false, false);
}

void Entity_ClearDestination(Entity* entity) {
	ClearNodeList(&entity->currentPath);
}




