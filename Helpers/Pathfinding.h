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

typedef struct NodeList NodeList;
typedef struct NodeList {
	Node data;
	NodeList* next;
	int length;
} NodeList;

#include "../Dungeon/DungeonLevel.h"
#include <string.h>
#include "Drawing.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ncurses/ncurses.h>

int NodeListLength(NodeList* q);

NodeList* ClearNodeList(NodeList *list);
void FreeNodeList(NodeList *list);

NodeList* AppendToNodeList(NodeList *q, Node n);

NodeList* PopNodeList(NodeList *q, int index, Node* out);
// pop lowest F score from a List
NodeList* PopLowestFScore(NodeList *q, int goalY, int goalX, Node* outNode);

// find the node in the List that matches the x-y coordinates of the input node
int FindNodeIndex(NodeList *q, Node *n);

NodeList* Backtrace(DungeonLevel *level, NodeList *closed, Node goalNode, int fromY, int fromX, int toY, int toX);

NodeList* Pathfind(DungeonLevel *level, int fromY, int fromX, int toY, int toX, bool ignoreCaveWall, bool ignoreDoors);

#endif