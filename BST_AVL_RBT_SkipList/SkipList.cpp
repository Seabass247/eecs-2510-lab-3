#include "SkipList.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <iostream>

using namespace std;

SkipList::SkipList()
{
	head = new SkipListNode(); // create the negative infinity sentinel node
	head->key = NEG_INF;
	tail = new SkipListNode(); // create the positive infinity sentinel node
	tail->key = POS_INF;
	head->right = tail; // link them to...
	tail->left = head; // ... each other
	n = 0; // no entries yet (empty list)
	h = 1; // no new levels yet, just a single level
	coin = mt19937(time(NULL)); // seed the RNG with a unique seed as to generate new "random" sequences every time
}

void SkipList::Insert(const char* X)
{
	bool found = false;
	SkipListNode* p = search(X, found);
	
	if (found) // The node is already in the tree, increment its count
	{
		p->count++;
		return;
	}

	SkipListNode* Y = new SkipListNode();
	Y->key = X;

	// Splice in Y in between p (immediate left of Y) and q (immediate right of Y)
	SkipListNode* q = p->right; 
	Y->left = p; // Adjust pointers to splice in the new node.
	Y->right = q;
	p->right = Y;
	q->left = Y;

	int level = 1;
	while (coin() & 1) // Approximately half the time, create a node in the level above
	{
		if (level >= h)
		{
			// Create a new top level
			// Create the new level's head and tail nodes, link them together 
			SkipListNode* pNew = new SkipListNode();
			pNew->key = NEG_INF;
			SkipListNode* qNew = new SkipListNode();
			qNew->key = POS_INF;
			pNew->right = qNew; // link them...
			qNew->left = pNew; // ...together

			head->up = pNew; // the new head is above the old head
			tail->up = qNew; // the new tail is above the old tail

			pNew->down = this->head; // old head is below the new head

			qNew->left = pNew; 
			qNew->down = this->tail; // old tail is below the new tail
			statPointerChange += 7;

			head = pNew; // update head...
			tail = qNew; // ... and tail
			// END adding new layer
			h++;
		}

		// Find first element with an up pointer
		while (p->left != NULL)
			p = p->left;
		if (p->up != NULL)
			p = p->up;
		// p will now either point to a node in the next level or a sentinel node (which means
		// the next   higher level was not created in this pass)

		// Configure pointers of new node Z
		SkipListNode* Z = new SkipListNode(); // Node Z is the extra-level node being inserted 

		Z->key = X;
		Z->left = p;
		Z->right = p->right;
		Z->down = Y;

		// Change the neighboring links
		p->right->left = Z;
		p->right = Z;
		Y->up = Z;
		statPointerChange++;

		Y = Z; // remember Z next time as Y 

		level++; // one more level has been added to the list
	}

	n++; // one more entry in the list
}

SkipList::SkipListNode* SkipList::search(const char* word, bool& found)
{
	SkipListNode* P = head; // P starts at the head

	while (true) // Repeat until we return out of the loop
	{
		while (strcmp(P->right->key, POS_INF) != 0) // While what's to the right of P is not a sentinel node
		{
			int compareValue = strcmp(P->right->key, word);
			statKeyComparison += 2;
			if (compareValue < 0)
			{
				P = P->right; // move p to the right
			}
			else if (compareValue == 0)
			{
				P = P->right; // move p to the right
				while (P->down != NULL) // go down to the slow lane
					P = P->down;
				// we found the node, return its pointer
				found = true;
				return P;
			}
			else break;
		}

		// We've broken out of the loop without returning or it never executed
		if (P->down == NULL) // In the slow lane?
		{
			found = false; // Could not find the node...
			return P; // ... however, return the pointer to some node that would preceed the one we've found.
		}

		// Otherwise, drop down a level and repeat
		P = P->down;
	}
}

void SkipList::List()
{
	cout << "SkipList contains: " << endl;
	SkipListNode* p = head;
	while (p->down != NULL) // move p to bottom-most node in left sentinel pillar
		p = p->down;
	p = p->right;
	while (p->key != POS_INF)
	{
		SkipListNode* q = p;
		do
		{
			cout << q->key << "x" << q->count << " ";
			q = q->up;
		} while (q != NULL);
		cout << endl;
		p = p->right;
	}
}

void SkipList::DisplayStatistics()
{
	int height = getListHeight();
	int distinctNodes = 0;
	int totalNodes = 0;
	int* nodesInlevel = new int[height];
	traverse(distinctNodes, totalNodes, nodesInlevel);
	int fastLaneNodes = totalNodes - distinctNodes;

	cout << "SkipList_slow_lane_nodes=" << distinctNodes << endl;
	cout << "SkipList_fast_lane_nodes=" << fastLaneNodes << endl;
	cout << "SkipList_height=" << height << endl;
	cout << "SkipList_distinct_nodes=" << distinctNodes << endl;
	cout << "SkipList_total_nodes=" << totalNodes << endl;
	cout << "SkipList_key_comparisons=" << statKeyComparison << endl;
	cout << "SkipList_pointer_changes=" << statPointerChange << endl;
	for (int i = 0; i < height; i++) // List the no. of nodes in each level starting from level 1 (the top)...
		// ...to level h (where h=height), each outputted on their own new line
	{
		int level = i + 1;
		cout << "SkipList_nodes_lane_" << level << "=" << nodesInlevel[i] <<endl;
	}
}

void SkipList::traverse(int& distinctNodes, int& totalNodes, int* nodesInLevel)
{
	SkipListNode* p = head;
	int measuredHeight = 1;
	while (p->down != NULL) // move p to bottom-most node in left sentinel pillar
	{
		p = p->down;
		measuredHeight++;
	}
	
	p = p->right;
	while (p->key != POS_INF)
	{
		SkipListNode* q = p;
		// Do something per unique node
		distinctNodes++;
		do
		{
			// Do something for every level of that node
			++totalNodes;
			q = q->up;
		} while (q != NULL);
		p = p->right;
	}
	
	p = head;
	SkipListNode* q = p;
	int level = 0; // level 0 will be the top-most level
	while (q != NULL) // Do something for every level (starting from the top)
	{
		int nodeCount = 0;
		p = q->right;
		while (strcmp(p->key, POS_INF) != 0) // Do something for every node in that level
		{
			nodeCount++;
			p = p->right;
		}
		q = q->down;
		nodesInLevel[level++] = nodeCount;
	}
}