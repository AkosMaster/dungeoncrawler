#include "Entity.h"

void Entity_OnTurn(Entity* entity) {
	if (entity->onTurn) {
		entity->onTurn(entity);
	}
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