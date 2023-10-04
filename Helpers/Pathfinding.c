#include "Pathfinding.h"

bool IsNodeListInit(NodeList *list) {
	return list->initCheck == NODELIST_INITCHECK;
}

void InitNodeList(NodeList *list) {
	list->size = 10;
	list->length = 0;
	list->items = (Node*)malloc(sizeof(Node) * list->size);
	list->initCheck = NODELIST_INITCHECK;
}

void ClearNodeList(NodeList *list) {
	list->size = 10;
	list->length = 0;
	list->items = (Node*)realloc(list->items, sizeof(Node) * list->size);
}

void FreeNodeList(NodeList *list) {
	list->size = 0;
	list->initCheck = 0;
	free(list->items);
}

bool AppendToNodeList(NodeList *q, Node n) {
	if (!IsNodeListInit(q)) {
		printf("ERROR: UNINITIALIZED NODELIST\n");
		exit(1);
	}

	if (q->length >= q->size) {
		q->size *= 2;
		q->items = (Node*)realloc(q->items, sizeof(Node)*q->size);
	}

	q->items[q->length] = n;
	q->length++;

	if (q->length > NODELIST_MAX) {
		printf("WARNING: NODELIST MAX EXCEEDED: %d\n", q->length);
		exit(1);
	}
	return true;
}

Node PopNodeList(NodeList *q, int index) {
	if (!IsNodeListInit(q)) {
		printf("ERROR: UNINITIALIZED NODELIST\n");
		exit(0);
	}

	Node poppedNode = q->items[index];
	memmove(&q->items[index], &q->items[index+1], (q->length-index-1)*sizeof(Node));
	q->length--;
	return poppedNode;
}

// pop lowest F score from a List
Node PopLowestFScore(NodeList *q, int goalY, int goalX) {
	
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

	return PopNodeList(q, bestIndex);
}

// find the node in the List that matches the x-y coordinates of the input node
int FindNodeIndex(NodeList *q, Node *n) {
	for (int i = 0; i < q->length; i++) {
		if (q->items[i].y == n->y && q->items[i].x == n->x) {
			return i;
		}
	}
	return -1; // not found
}

void Backtrace(DungeonLevel *level, NodeList *outPath, NodeList *closed, Node goalNode, int fromY, int fromX, int toY, int toX) {

	Node node = goalNode;

	while (node.y != fromY || node.x != fromX) {

		AppendToNodeList(outPath, node);

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
	AppendToNodeList(outPath, node);
}

void Pathfind(DungeonLevel *level, NodeList *outPath, int fromY, int fromX, int toY, int toX, bool ignoreCaveWall, bool ignoreDoors) {

	ClearNodeList(outPath); // clear output list

	Node startNode = {.y=fromY, .x=fromX, .g=0, .parentY=0, .parentX=0};
	
	NodeList OpenList;
	InitNodeList(&OpenList);
	NodeList ClosedList;
	InitNodeList(&ClosedList);

	AppendToNodeList(&OpenList, startNode);

	// based on https://mat.uab.cat/~alseda/MasterOpt/AStar-Algorithm.pdf
	int iterations = 0;
	while (OpenList.length > 0 && ClosedList.length < NODELIST_MAX && OpenList.length < NODELIST_MAX) {
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
				PopNodeList(&OpenList, openListIndex);
				AppendToNodeList(&OpenList, successorNode);
			} else if (closedListIndex != -1) {
				if (ClosedList.items[closedListIndex].g <= successorNode.g)
					continue; // already closed, and we didnt find a better path to it

				// Move from Closed to Open since we found a better path
				PopNodeList(&ClosedList, closedListIndex);
				AppendToNodeList(&OpenList, successorNode);
			} else {

				AppendToNodeList(&OpenList, successorNode);
			}
		}
		AppendToNodeList(&ClosedList, currentNode);
	}
	FreeNodeList(&OpenList);
	FreeNodeList(&ClosedList);
}