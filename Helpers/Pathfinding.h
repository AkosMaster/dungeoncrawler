#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "../Dungeon/DungeonLevel.h"
#include <string.h>
#include "Drawing.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ncurses/ncurses.h>

typedef struct Node
{
	int y;
	int x;
	int g; // best distance from origin

	int parentY;
	int parentX;
} Node;

#define LIST_MAX 500
typedef struct List {
	int length;
	Node items[LIST_MAX];
} List;

static bool AppendToList(List *q, Node n) {
	if (q->length >= LIST_MAX) {
		printf("ERROR: Pathfinding List too long!\n");
		exit(1); // TODO handle this
		return false;
	}

	q->items[q->length] = n;
	q->length++;
	return true;
}

static Node PopList(List *q, int index) {
	Node poppedNode = q->items[index];
	memmove(&q->items[index], &q->items[index+1], (q->length-index-1)*sizeof(Node));
	q->length--;
	return poppedNode;
}

// pop lowest F score from a List
static Node PopLowestFScore(List *q, int goalY, int goalX) {
	
	int lowestF = 99999999;
	int bestIndex = 0;
	for (int i = 0; i < q->length; i++) {
		Node node = q->items[i];

		int heuristic = pow(goalY - node.y, 2) + pow(goalX - node.x, 2);

		int f = node.g + heuristic;

		if (f < lowestF) {
			lowestF = f;
			bestIndex = i;
		}
	}

	return PopList(q, bestIndex);
}

// find the node in the List that matches the x-y coordinates of the input node
static int FindNodeIndex(List *q, Node *n) {
	for (int i = 0; i < q->length; i++) {
		if (q->items[i].y == n->y && q->items[i].x == n->x) {
			return i;
		}
	}
	return -1; // not found
}

static void Backtrace(DungeonLevel *level, List *outPath, List *closed, Node goalNode, int fromY, int fromX, int toY, int toX) {

	Node node = goalNode;

	while (node.y != fromY || node.x != fromX) {

		AppendToList(outPath, node);

		Node parentNode;
		parentNode.y = node.parentY;
		parentNode.x = node.parentX;

		int parentIndex = FindNodeIndex(closed, &parentNode);

		if (parentIndex == -1 || parentIndex >= closed->length) {
			printf("Path could not be backtraced!\n");
			exit(1); // TODO handle this
			return;
		} else {
			node = closed->items[parentIndex];
		}
	}
	// append end position
	AppendToList(outPath, node);
}

static void Pathfind(DungeonLevel *level, List *outPath, int fromY, int fromX, int toY, int toX, bool ignoreCaveWall, bool ignoreDoors) {

	outPath->length = 0; // clear output list

	Node startNode = {.y=fromY, .x=fromX, .g=0, .parentY=0, .parentX=0};
	
	List OpenList = {.length=0};
	List ClosedList = {.length=0};

	AppendToList(&OpenList, startNode);

	// based on https://mat.uab.cat/~alseda/MasterOpt/AStar-Algorithm.pdf
	int iterations = 0;
	while (OpenList.length > 0 && ClosedList.length < LIST_MAX-1) {
		Node currentNode = PopLowestFScore(&OpenList, toY, toX);

		if (currentNode.y == toY && currentNode.x == toX) {

			Backtrace(level, outPath, &ClosedList, currentNode, fromY, fromX, toY, toX);
			break;
		}

		// enumerate all 4 neighbours
		for (int neighbor = 0; neighbor < 4; neighbor++) {
			Node successorNode = {.y=currentNode.y, .x=currentNode.x, .g=currentNode.g + 1, .parentY=currentNode.y, .parentX=currentNode.x};
			switch (neighbor) {
				case 0:
					successorNode.x += 1;
					break;
				case 1:
					successorNode.x -= 1;
					break;
				case 2:
					successorNode.y += 1;
					break;
				case 3:
					successorNode.y -= 1;
					break;
			}

			if (successorNode.y < 0 || successorNode.y >= LevelHeight || successorNode.x < 0 || successorNode.x >= LevelWidth) {
				continue;
			}

			
			if (!level->tiles[successorNode.y][successorNode.x].walkable) {
				if (!(level->tiles[successorNode.y][successorNode.x].id == CaveWall.id && ignoreCaveWall)) {
					if (!((level->tiles[successorNode.y][successorNode.x].id == OpenDoor.id || level->tiles[successorNode.y][successorNode.x].id == ClosedDoor.id) && ignoreDoors)) {
						continue;
					}
				}
			}

			int openListIndex = FindNodeIndex(&OpenList, &successorNode);
			int closedListIndex = FindNodeIndex(&ClosedList, &successorNode);

			if (openListIndex != -1) {
				if (OpenList.items[openListIndex].g <= successorNode.g)
					continue; // the successor is already in the open List, and our path to it is not better than the previous.

				// replace previous entry in open List since we found a better path
				PopList(&OpenList, openListIndex);
				AppendToList(&OpenList, successorNode);

			} else if (closedListIndex != -1) {
				if (ClosedList.items[closedListIndex].g <= successorNode.g)
					continue; // already closed, and we didnt find a better path to it

				// Move from Closed to Open since we found a better path
				PopList(&ClosedList, closedListIndex);
				AppendToList(&OpenList, successorNode);
			} else {
				AppendToList(&OpenList, successorNode);
			}
		}

		AppendToList(&ClosedList, currentNode);
	}
}

#endif