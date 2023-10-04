#ifndef PATHFINDING_H
#define PATHFINDING_H

typedef struct Node
{
	int y;
	int x;
	int g; // best distance from origin

	int parentY;
	int parentX;
} Node;

#define NODELIST_MAX 500
#define NODELIST_INITCHECK 1337
typedef struct NodeList {
	int length;
	int size; // dynamically growing array
	Node* items;
	int initCheck; // for avoiding stupid errors
} NodeList;

#include "../Dungeon/DungeonLevel.h"
#include <string.h>
#include "Drawing.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ncurses/ncurses.h>

bool IsNodeListInit(NodeList *list);

void InitNodeList(NodeList *list);
void ClearNodeList(NodeList *list);
void FreeNodeList(NodeList *list);

bool AppendToNodeList(NodeList *q, Node n);

Node PopNodeList(NodeList *q, int index);
// pop lowest F score from a List
Node PopLowestFScore(NodeList *q, int goalY, int goalX);

// find the node in the List that matches the x-y coordinates of the input node
int FindNodeIndex(NodeList *q, Node *n);

void Backtrace(DungeonLevel *level, NodeList *outPath, NodeList *closed, Node goalNode, int fromY, int fromX, int toY, int toX);

void Pathfind(DungeonLevel *level, NodeList *outPath, int fromY, int fromX, int toY, int toX, bool ignoreCaveWall, bool ignoreDoors);

#endif