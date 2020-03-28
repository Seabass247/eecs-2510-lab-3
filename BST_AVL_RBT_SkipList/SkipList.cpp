#include "SkipList.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <iostream>

using namespace std;

SkipList::SkipList()
{
	head = new SkipListNode(); // create the negative infinity sentinel node
	head->key = NEG_INF; // set its identity to sentinel
	tail = new SkipListNode(); // create the positive infinity sentinel node
	tail->key = POS_INF; // set its identify it to sentinel
	head->right = tail; // link them to...
	tail->left = head; // ... each other
	n = 0; // no entries yet (empty list)
	h = 1; // no new levels yet, just a single level
	srand(time(NULL)); // seed the RNG with a unique seed as to generate new "random" sequences every time
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

	SkipListNode* lastInserted = Y;
	int level = 0;
	while (rand() & 1) // Approximately half the time, create a node in the level above
	{
		SkipListNode* Z = lastInserted; // Create Y's respective next-level node, Z.

		while (p->up != NULL)
		{
			if (Z->up != NULL) // Does this node have a node above it?
			{
				p = Z->up;
				break;
			}
			Z = Z->left;
		} // Now p will either be in the level above Y/Z or a sentinel node.

		
		Z = new SkipListNode(); // Reinitialize Z as to not modify Y.

		if ((strcmp(p->key,NEG_INF) == 0) && h != 0) // Is p sentinel (there's NOT a next level above p)
		{
			// Add a new level and insert X in it
			SkipListNode* pNew = new SkipListNode();
			pNew->key = NEG_INF;
			SkipListNode* qNew = new SkipListNode();
			qNew->key = POS_INF;

			pNew->down = head; head->up = pNew; // Node linkage for new level...
			qNew->down = tail; tail->up = qNew;
			pNew->right = qNew; qNew->left = pNew;
			head = pNew; tail = qNew; // ...
			h++; // List is now one level taller
			level++; // TODO REMOVE
			// Splice Z in between p and q 
			Z->left = pNew; // Adjust pointers to splice in the new node.
			Z->right = qNew;
			Z->down = lastInserted;
			lastInserted->up = Z;
			Z->key = X;
			Z->level = level;
			pNew->right = Z;
			qNew->left = Z;
			lastInserted = Z;
		}
		else // p is some node in the level above the newly inserted node Y
		{
			cout << "p_key=" << p->key << endl;
			// The next level exists. Adjust pointers to splice in the new node.
			// Insert the node at this level
			Z->key = X;
			Z->level = level;
			q = p->right; // q will be the node immediately right of Z

			Z->left = p; // Adjust pointers to splice in the new node.
			Z->right = q;
			Z->down = Y;
			lastInserted->up = Z;
			cout << "lastInserted " << lastInserted->key << "l" << lastInserted->level << ".up = " << Z->key << "l" << Z->level <<endl;
			p->right = Z;
			q->left = Z;
			lastInserted = Z;
		}
	}
}

SkipList::SkipListNode* SkipList::search(const char* word, bool& found)
{
	SkipListNode* P = head; // P starts at the head
	while (true) // Repeat until we return out of the loop
	{
		while ((P->right->key != NEG_INF) && (P->right->key != POS_INF)) // While what's to the right of P is not a sentinel node
		{
			int compareValue = strcmp(P->right->key, word);
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
	SkipListNode* p = head;
	while (p->down != NULL) // move p to bottom-most node in left sentinel pillar
		p = p->down;
	p = p->right;
	while (p->key != POS_INF)
	{
		SkipListNode* q = p;
		do
		{
			cout << q->key << "x"<< q->count << " ";
			q = q->up;
		} 
		while (q != NULL);
		cout << endl;
		p = p->right;
	}
}

void SkipList::Height()
{
	cout << "SkipList height=" << h << endl;
}
