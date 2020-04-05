// SkipList.cpp
// Sebastian Hamel
// EECS 2510, Spring 2020
// 04/03/2020
// Contains the implementation of the SkipList class. Enables the Skip List to have words inserted and exposes the 
// resulting statistics via DisplayStatistics.
#include "SkipList.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <iostream>

using namespace std;

// The Skip List's constructor. Configures a new Skip List where the head and tail are sentinel nodes
SkipList::SkipList()
{
	head = new SkipListNode(); // create the negative infinity sentinel node
	strcpy_s(head->key, NEG_INF);
	tail = new SkipListNode(); // create the positive infinity sentinel node
	strcpy_s(tail->key, POS_INF);
	head->right = tail; // link them to...
	tail->left = head; // ... each other
	n = 0; // no entries yet (empty list)
	h = 1; // no new levels yet, just a single level
	coin = mt19937(time(NULL)); // seed the RNG with a unique seed as to generate new "random" sequences every time
}

// Inserts X into the list. It might randomly create more than one node in the list in a single insertion
// to put the new node into the "fast lanes."
void SkipList::Insert(const char* X)
{
	bool found = false;
	SkipListNode* p = search(X, found); // let p point to either the node with key X or the node with the next highest key value...
										// ...before X
	if (found) // The node is already in the tree, increment its count
	{
		p->count++;
		return;
	}

	SkipListNode* Y = new SkipListNode();
	strcpy_s(Y->key, X);

	// Splice in Y in between p (immediate left of Y) and q (immediate right of Y)
	SkipListNode* q = p->right; 
	Y->left = p; // Adjust pointers to splice in the new node.
	Y->right = p->right;
	statPointerChange += 2;

	p->right->left = Y;
	p->right = Y;
	statPointerChange += 2;


	statTossedCoin++;
	int level = 1;
	while (coin() & 1) // Approximately half the time, create a node in the level above
	{
		statTossedCoin++;
		statTossedHeads++;
		level++; // one more level has been added to the list

		if (level > h)
		{
			// Create a new top level
			// Create the new level's head and tail nodes, link them together 
			SkipListNode* pNew = new SkipListNode();
			strcpy_s(pNew->key, NEG_INF);
			SkipListNode* qNew = new SkipListNode();
			strcpy_s(qNew->key, POS_INF);

			pNew->right = qNew; // link them...
			qNew->left = pNew; // ...together

			head->up = pNew; // the new head is above the old head
			tail->up = qNew; // the new tail is above the old tail

			pNew->down = head; // old head is below the new head

			qNew->left = pNew; 
			qNew->down = tail; // old tail is below the new tail

			head = pNew; // update head...
			tail = qNew; // ... and tail
			
			statPointerChange += 9;

			// END adding new layer
			h++;
		}

		// Find first element with an up pointer
		while (p->up == NULL)
			p = p->left;
		p = p->up;

		// p will now either point to a node in the next level or a sentinel node (which means
		// the next   higher level was not created in this pass)

		// Configure pointers of new node Z
		SkipListNode* Z = new SkipListNode(); // Node Z is the extra-level node being inserted 

		strcpy_s(Z->key, X);
		Z->left = p;
		Z->right = p->right;
		Z->down = Y;
		statPointerChange += 3;

		// Change the neighboring links
		p->right->left = Z;
		p->right = Z;
		Y->up = Z;
		statPointerChange += 3;

		Y = Z; // remember Z next time as Y 
	}

	n++; // one more entry in the list
}

// Traverses the list iteratively starting from the top, searching for the node with the same 
// value as X, setting found = true if X is found, otherwise returns the next highest value node
// before X and sets found = false.
SkipList::SkipListNode* SkipList::search(const char* X, bool& found)
{
	SkipListNode* P = head; // P starts at the head
	int compareValue;

	while (true) // Repeat until we return out of the loop
	{
		compareValue = strcmp(X, P->right->key);
		statKeyComparison++; 
		if (compareValue == 0)
		{
			P = P->right;
			while (P->down != NULL) // move p to bottom-most node
				P = P->down;
			found = true; // this nodes already here
			return P;
		}
		if (compareValue > 0 && P->right->right != NULL) // The node to the right is greater and is not sentinel
		{
			P = P->right; // Go right
			continue;
		}

		if (P->down == NULL) // In the slow lane?
		{
			found = false; // Could not find the node...
			return P;
		}

		// Otherwise, drop down a level and repeat
		P = P->down;
	}
}

// Outputs the nodes in the list in every level for debug purposes.
void SkipList::List()
{
	cout << "SkipList contains: " << endl;
	SkipListNode* p = head;
	while (p->down != NULL) // move p to bottom-most node in left sentinel pillar
		p = p->down;
	p = p->right;
	while (strcmp(p->key, POS_INF) != 0)
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
	cout << endl;
}

// Outputs the Skip List's statistics in comma-separated values (CSV)
// Gets the height via getListHeight, node counts via Traverse, and every other metric
// via class-wide variables.
void SkipList::DisplayStatistics()
{
	int height = getListHeight(); // get the list height found by a traversal
	int distinctCount = 0;
	int totalNodes = 0;
	int totalWords = 0;
	int* nodesInlevel = new int[height]; // used to store the number of nodes in every level of the list
	traverse(distinctCount, totalNodes, totalWords, nodesInlevel);
	int fastLaneNodes = totalNodes - distinctCount;

	// Output everything to the console CSV style
	cout << "SKIPLIST STATISTICS:" << endl << endl;
	cout << "Slow lane nodes," << distinctCount << endl;
	cout << "fast lane nodes," << fastLaneNodes << endl;
	cout << "Height," << height << endl;
	cout << "Distinct words," << distinctCount << endl;
	cout << "Total words," << totalWords << endl;
	cout << "Total list nodes," << totalNodes << endl;
	cout << "Key comparisons," << statKeyComparison << endl;
	cout << "Pointer changes," << statPointerChange << endl;
	cout << "Tossed coin," << statTossedCoin << endl;
	cout << "Tossed heads," << statTossedHeads << endl;
	for (int i = 0; i < height; i++) // List the no. of nodes in each level starting from level 1 (the top)...
		// ...to level h (where h=height), each outputted on their own new line
	{
		int level = i + 1;
		cout << "Nodes in lane " << level << "," << nodesInlevel[i] <<endl;
	}
}

// Traverses iteratively through the list starting from the head at the top-most level, moving the p pointer right
// as far as possible before moving down a level again.  Modifies the node count parameters based on the 
// nodes found during traversal.
void SkipList::traverse(int& distinctNodes, int& totalNodes, int& totalWords, int* nodesInLevel)
{
	SkipListNode* p = head;
	while (p->down != NULL) // move p to bottom-most node in left sentinel pillar
		p = p->down;
	p = p->right;
	while (p->right != NULL)
	{
		distinctNodes++;
		totalWords += p->count;
		//cout << "Distinct " << p->key << endl;
		p = p->right;
	}

	p = head;
	SkipListNode* q = p;
	int level = 0; // level 0 will be the top-most level
	while (q != NULL) // Do something for every level (starting from the top)
	{
		int nodeCount = 0;
		p = q->right;
		while (p->right != NULL) // Do something for every node in that level
		{
			nodeCount++;
			p = p->right;
		}
		q = q->down;
		nodesInLevel[level++] = nodeCount;
		totalNodes += nodeCount;
	}
}