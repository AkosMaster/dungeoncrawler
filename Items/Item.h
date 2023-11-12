#ifndef ITEM_H
#define ITEM_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct Item Item;
#include "../Entities/Entity.h"

// a targyak virtalis fuggvenyei, melyet az oroklo struktura tetszese szerint allithat be.
// lehet 0 is, ekkor ez a fuggveny nincs ertelmezve az adott targyra

typedef void (*Interaction_Reload)(Item* baseItem, Item* ammo);
typedef void (*Interaction_Attack)(Item* baseItem, int dirY, int dirX);
typedef void (*Interaction_Consume)(Item* baseItem);
typedef void (*DrawEffectsFunction)(Item* baseItem);
typedef void (*DeleteFunction)(Item* baseItem);

typedef struct Item {

	char name[15]; // nev
	char symbol; // szimbolum (ASCII)
	short foreColor; // szimbolum szin
	short backColor; // szimolum hatterszin

	bool stackable; // lehet-e tobb belole egy adott inventory helyen? Pl: aranypenzbol igen, fegyverekbol nem
	int stackSize; // ha stackable, akkor mennyi van belole?

	Entity* owner; // a birtoklo entitas
	
	DrawEffectsFunction drawEffects; // a targy extra effektjeit rajzolja ki
	DeleteFunction delete; // itt kell a targynak az extra memoriaigenyeitol megszabadulni

	//interakciok
	Interaction_Reload interact_Reload; // ujratolt
	Interaction_Attack interact_Attack; // tamadas a targgyal
	Interaction_Consume interact_Consume; // elfogyasztás

} Item;

const static Item defaultItem = {
	.name="default",
	.symbol='?',
	.foreColor=0,
	.backColor=0,

	.stackable=false,
	.stackSize=1,

	.drawEffects=0,
	.interact_Reload=0,
	.interact_Consume=0,
};

// felhozza az interakcio menut a targyhoz, kiirja az osszes lehetseges interakciot.
// csak a Player-nek kell ezt meghivni
void ItemInteractMenu(Item* item);

// kitorli az adott targyat a memoriabol
void Item_Delete(Item* item);

// kirajzolja a targy effektjeit (pl: loves, robbanas, stb)
void Item_DrawEffects(Item* item);

// Interakcio: ujratoltes
void Item_Interact_Reload(Item* item, Item* ammo);

// Interakcio: tamadas a targgyal
void Item_Interact_Attack(Item* item, int dirY, int dirX);

void Item_Interact_Consume(Item* item);
#endif