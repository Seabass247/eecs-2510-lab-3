#include "SkipList.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <iostream>

using namespace std;

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

void SkipList::Insert(char X[50])
{
	bool found = false;
	SkipListNode* p = search(X, found);

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
	
	int level = 1;
	while (coin() & 1) // Approximately half the time, create a node in the level above
	{
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
		statPointerChange++;
	}

	n++; // one more entry in the list
}

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

void SkipList::DisplayStatistics()
{
	int height = getListHeight();
	int distinctCount = 0;
	int totalNodes = 0;
	int totalWords = 0;
	int* nodesInlevel = new int[height];
	traverse(distinctCount, totalNodes, totalWords, nodesInlevel);
	int fastLaneNodes = totalNodes - distinctCount;

	cout << "SkipList_slow_lane_nodes=" << distinctCount << endl;
	cout << "SkipList_fast_lane_nodes=" << fastLaneNodes << endl;
	cout << "SkipList_height=" << height << endl;
	cout << "SkipList_distinct_words=" << distinctCount << endl;
	cout << "SkipList_total_words=" << totalWords << endl;
	cout << "SkipList_total_list_nodes=" << totalNodes << endl;
	cout << "SkipList_key_comparisons=" << statKeyComparison << endl;
	cout << "SkipList_pointer_changes=" << statPointerChange << endl;
	for (int i = 0; i < height; i++) // List the no. of nodes in each level starting from level 1 (the top)...
		// ...to level h (where h=height), each outputted on their own new line
	{
		int level = i + 1;
		cout << "SkipList_nodes_lane_" << level << "=" << nodesInlevel[i] <<endl;
	}
}

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