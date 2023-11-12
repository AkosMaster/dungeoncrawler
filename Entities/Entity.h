#ifndef ENTITY_H
#define ENTITY_H

typedef struct Entity Entity;

#include "../Items/Item.h"
#include "../Dungeon/DungeonLevel.h"
#include "../Helpers/Pathfinding.h"

// csapatok, ez adja meg mely entitasok ellensegesek/baratsagosak egymassal
typedef enum EntityTeam {
	None,
	DontAttack,
	Rats,
	Crawlers,
	Humanoids,
} EntityTeam;

// entitasok lehetseges status-effektjei.
typedef enum EntityStatusFX {
	FX_Frozen, // fagyott effekt (varazslat hatasara)

	// megadja a status effektek szamat
	EntityStatusFXCount
} EntityStatusFX;

// az entitasok virtalis fuggvenyei, melyet az oroklo struktura tetszese szerint allithat be.
// lehet 0 is, ekkor ez a fuggveny nincs ertelmezve az adott entitasra
typedef void (*OnTurnFunction)(Entity* baseEntity);
typedef void (*DrawFunction)(Entity* baseEntity);
typedef void (*DeSpawnFunction)(Entity* baseEntity);
typedef void (*DamageFunction)(Entity* baseEntity, Entity* attacker, int points);
typedef void (*Interaction_Loot)(Entity* baseEntity, Entity* looter);

typedef struct Entity {
	char symbol; // kirajzolt ASCII karakter
	short foreColor; // szimbolum szine
	short backColor; // szimbolum hatterszine

	int score; // pontok

	int health; // eletero
	char name[20]; // nev
	int speed; // a globalis "korok" hany szazalekaban kap az entitas lehetoseget hogy mozogjon?

	EntityTeam team; // csapat

	int movementPoints; // ez a speed valtozohoz szukseges. Lasd a Level implementacioban.

	// koordinatak
	int y;
	int x;
	DungeonLevel* level; // az entitast tartalmazo palya struktura

	// pointerek az entitas targyaihoz. Fixen 10 hely van, hiszen ennyi szamozott billentyu van a billentyuzeten.
	Item* inventory[10];

	// virtualis fuggvenyek
	OnTurnFunction onTurn;
	DrawFunction draw;
	DeSpawnFunction deSpawn;
	DamageFunction damage;

	// ezek a virtualis fuggvenyek a lehetseges interakciokat adjak meg entitasok kozott
	Interaction_Loot interact_Loot; // kifosztas

	// utkereses, ez a jelenleg kovetett utvonal (ha van, nem minden entitas hasznalja)
	NodeList currentPath;

	// statusz effektusok 
	int status_fx[EntityStatusFXCount];

} Entity;

// alaveto entitas, a Spawn_(valami) fuggvenyek ezt masoljak le es irjak at.
const static Entity defaultEntity = {
		.health=50, 
		.score=1,
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

// ha az entitas lehetoseget kap egy akciora, (speed hatarozza meg), akkor ez
// a fuggveny lesz meghivva. Egyes statusz effektek hatassal lehetnek erre (pl: Freeze)
void Entity_OnTurn(Entity* entity);

// ez minden globalis korben (vagy tick-ben) meghivodik, fokent a statusz effektek idejenek csokkentesere szol.
void Entity_OnGameTick(Entity* entity);

// az entitas kirajzolasa, ez meghivja az entitas kulon onDraw funkciojat is
void Entity_Draw(Entity* entity);

// Az entitas halalakor jelentos, free-eli az entitas memoriajat, illetve az osszes targyat mely hozza tartozott.
// Ez meghivja az onDeSpawn fuggvenyt, ahol az entitast oroklo objektumnak fel kell szabaditania a sajat memoriaigenyeit
void Entity_deSpawn(Entity* entity);

// Ha egy entitas megtamad egy masikat, akkor ez a fuggveny fut le.
void Entity_Damage(Entity* entity, Entity* attacker, int points);

// Interakcio: egy entitas kifoszt egy masikat.
void Entity_Interact_Loot(Entity* entity, Entity* looter);

// beallitja az entitas utkeresesenek celpontjat. Ezutan az entitas minden koreben ezt az utat fogja kovetni,
// amig ez meg nem valtozik vagy torlodik.
void Entity_SetDestination(Entity* entity, int y, int x);

// kitorli az entitas celpontjat, innentol nem fog utvonalat kovetni.
void Entity_ClearDestination(Entity* entity);

// hozzaad egy targyat az entitas inventory-hoz, es frissiti az item "owner" valtozojat
bool Entity_AddItemToInventory(Entity* entity, Item* item);

// elvesz egy targyat az inventory-bol, pointer alapjan. Az item regi "owner" erteke megmarad, de ez mar nem valid.
void Entity_RemoveItemFromInventory(Entity* entity, Item* item);
#endif




