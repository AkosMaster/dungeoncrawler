#include "../debugmalloc.h"

#include "Pathfinding.h"

int NodeListLength(NodeList* q) {
	if (q == NULL) {
		return 0;
	}
	int c = 0;
	while (q != NULL) {
		c++;
		q = q->next;
	}
	return c;
}

NodeList* AppendToNodeList(NodeList* q, Node node) {
	NodeList* new = (NodeList*)malloc(sizeof(NodeList));
	new->data = node;
	new->next = NULL;

	if (q == NULL) {
		return new;
	}

	NodeList* current = q;
	while (current->next != NULL){
		current = current->next;
	}
	current->next = new;

	//printf("append\n");

	return q;
}

void FreeNodeList(NodeList* q) {
	if (q == NULL) {
		return;
	} else {
		FreeNodeList(q->next);
		free(q);
	}
}

NodeList* ClearNodeList(NodeList* q) {
	FreeNodeList(q);
	return NULL;
}

Node GetNodeAtIndex(NodeList* q, int index) {
	int c = 0;
	while (c < index) {
		q = q->next;
		c++;
	}
	//printf("getatidx\n");
	return q->data;
}

NodeList* PopNodeList(NodeList* q, int index, Node* outNode) {
	if (index == 0) {
		*outNode = q->data;
		NodeList* newFirst = q->next;
		free(q);
		//printf("pop\n");
		return newFirst;
	}

	q->next = PopNodeList(q->next, index-1, outNode);
	
	return q;
}

// pop lowest F score from a List
NodeList* PopLowestFScore(NodeList *q, int goalY, int goalX, Node* outNode) {
	
	int lowestF = 99999999;
	int bestIndex = 0;
	int c = 0;
	NodeList* current = q;
	while (current != NULL) {
		Node node = current->data;

		int heuristic = pow(goalY - node.y, 2) + pow(goalX - node.x, 2);

		int f = node.g + heuristic;

		if (f < lowestF) {
			lowestF = f;
			bestIndex = c;
		}
		current = current->next;
		c++;
	}

	return PopNodeList(q, bestIndex, outNode);
}

// find the node in the List that matches the x-y coordinates of the input node
int FindNodeIndex(NodeList *q, Node *n) {
	NodeList* current = q;
	int i = 0;
	while (current != NULL) {
		//printf("	current: %p", current);
		if (current->data.y == n->y && current->data.x == n->x) {
			return i;
		}
		i++;
		current = current->next;
	}
	return -1; // not found
}

NodeList* Backtrace(DungeonLevel *level, NodeList *closed, Node goalNode, int fromY, int fromX, int toY, int toX) {

	//printf("btrace\n");

	NodeList* outPath = NULL;

	Node node = goalNode;

	while (node.y != fromY || node.x != fromX) {

		outPath = AppendToNodeList(outPath, node);

		Node parentNode;
		parentNode.y = node.parentY;
		parentNode.x = node.parentX;

		int parentIndex = FindNodeIndex(closed, &parentNode);

		if (parentIndex == -1 || parentIndex >= closed->length) {
			printf("Path could not be backtraced!\n");
			exit(1); // TODO handle this
			return NULL;
		} else {
			node = GetNodeAtIndex(closed, parentIndex);//closed->items[parentIndex];
		}
	}
	// append end position
	outPath = AppendToNodeList(outPath, node);

	return outPath;
}

NodeList* Pathfind(DungeonLevel *level, int fromY, int fromX, int toY, int toX, bool ignoreCaveWall, bool ignoreDoors) {

	//printf("pfind\n");

	Node startNode = {.y=fromY, .x=fromX, .g=0, .parentY=0, .parentX=0};
	
	NodeList* OpenList = NULL;
	NodeList* ClosedList = NULL;

	OpenList = AppendToNodeList(OpenList, startNode);

	NodeList* outList = NULL;

	// based on https://mat.uab.cat/~alseda/MasterOpt/AStar-Algorithm.pdf
	while (NodeListLength(OpenList) > 0 && NodeListLength(ClosedList) < NODELIST_MAX && NodeListLength(OpenList) < NODELIST_MAX) {
		Node currentNode;
		OpenList = PopLowestFScore(OpenList, toY, toX, &currentNode);

		//printf("asd\n");

		if (currentNode.y == toY && currentNode.x == toX) {
			outList = Backtrace(level, ClosedList, currentNode, fromY, fromX, toY, toX);
			break;
		}

		// enumerate all 4 neighbours
		for (int neighbor = 0; neighbor < 4; neighbor++) {
			//printf("asd0.5\n");
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

			//printf("Open: %p, closed: %p\n", OpenList, ClosedList);

			int openListIndex = FindNodeIndex(OpenList, &successorNode);
			int closedListIndex = FindNodeIndex(ClosedList, &successorNode);

			//printf("\nasd2\n");

			Node dontNeed;
			if (openListIndex != -1) {
				if (GetNodeAtIndex(OpenList, openListIndex).g <= successorNode.g)
					continue; // the successor is already in the open List, and our path to it is not better than the previous.

				// replace previous entry in open List since we found a better path
				OpenList = PopNodeList(OpenList, openListIndex, &dontNeed);
				OpenList = AppendToNodeList(OpenList, successorNode);
			} else if (closedListIndex != -1) {
				if (GetNodeAtIndex(ClosedList, closedListIndex).g <= successorNode.g)
					continue; // already closed, and we didnt find a better path to it

				// Move from Closed to Open since we found a better path
				ClosedList = PopNodeList(ClosedList, closedListIndex, &dontNeed);
				OpenList = AppendToNodeList(OpenList, successorNode);
			} else {

				OpenList = AppendToNodeList(OpenList, successorNode);
			}
			//printf("asd3\n");
		}
		ClosedList = AppendToNodeList(ClosedList, currentNode);
	}
	FreeNodeList(OpenList);
	FreeNodeList(ClosedList);

	return outList;
}