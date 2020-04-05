// AVL.cpp
// Sebastian Hamel
// EECS 2510, Spring 2020
// 04/03/2020
// Contains the implementation of the AVL class. Enables the AVL tree to have words inserted and exposes the 
// resulting statistics via DisplayStatistics.
#include "AVL.h"
#include <iostream>

using namespace std;

// The AVL tree's constructor. Configures a new AVL tree where the root is null;
// the tree is empty by default.
AVL::AVL()
{
	root = NULL;
}

// Inserts a new value into the AVL Tree. If the value is already in the tree,
// increment the count of the appropriate node by 1 and return.  Otherwise,
// initialize a new node, find an appropriate place to insert it based on the 
// value of its key, then check for any imbalances and rerrange the offending
// subtrees to decrease any balance factors of magnitude 2.
void AVL::Insert(const char* X)
{
	node* Y; // The new node we insert (z in the previous code)
	node* A, * B; // A will be the last parent above Y with a BF of ±1 (before the insert)
	node* F; // F is A’s parent (F lags one step behind A)
	node* P; // P will be used to scan through the tree until it falls off bottom. 
	node* Q; // Q lags behind P, so it will be the parent of Y when P falls off.
	node* C, * CL, * CR;
	int  d;             // displacement; Used to adjust BFs

	if (root == NULL)   // Empty tree? Make a root node and exit!
	{
		Y = new node(X);    // make and fill a node
		Y->LCH = Y->RCH = NULL;  // leaf --> no children
		Y->BF = 0;     // it is, by definition, balanced
		root = Y;        // root was NULL, so Y is new root
		statPointerChange += 4;
		statNoRotation++;
		return;          // This was the trivial case
	}

	//
	// Locate insertion point for X.
	//
	F = Q = NULL; // F and Q lag, so they start NULL
	A = P = root; // A and P start at the root

	while (P != NULL) // search tree for insertion point
	{
		int comparisonValue = strcmp(X, P->data);
		statKeyComparison++;
		if (comparisonValue == 0) {
			P->count++;
			return;  // ALREADY HERE!
		}
		if (P->BF != 0) // remember the last place we saw
		{
			A = P; F = Q;
		}  // a non-zero BF (and its parent)
		Q = P;                               // Bring Q up to where P is
		P = (comparisonValue < 0) ? P->LCH : P->RCH; // and then advance P (based on BST rule to go L or R).
	}

	statNoRotation++;
	//
	// At this point, P is NULL, but Q points at the last node where X
	// belongs (either as Q’s LCH or RCH, and Q points at an existing leaf)
	//
	Y = new node(X);   // Make a new node
	Y->LCH = NULL; // New nodes are always inserted...
	Y->RCH = NULL; // ...as leaves (i.e., no children)
	Y->BF = 0;    // Leaves are always balanced by definition


	// Will Y be Q's new left or right child?
	if (strcmp(X, Q->data) < 0) Q->LCH = Y;
	else Q->RCH = Y;
	statPointerChange++;

	//
	// Detect (and fix, if we have) an imbalance
	//
	// Adjust BFs from A to Q. 
	// A was the LAST ancestor with a BF of ± 1, (or is still the root,
	// because we never FOUND a BF of ± 1), so ALL nodes BETWEEN A and Q 
	// must have a BF of 0, and will, therefore, BECOME ± 1.
	//
	// If X is inserted in the LEFT subtree of A, then d = +1 (d = -1 means
	// we inserted X in the RIGHT subtree of A.
	statKeyComparison++;
	if (strcmp(X, A->data) > 0) { B = P = A->RCH; d = -1; } // Which way is the displacement (d)
	else { B = P = A->LCH; d = +1; } // B is identified as A’s child
	
	
	while (P != Y)  // P is now one node below A.  Adjust from here to the
	{               // insertion point.  Don’t do anything to new node (Y)
		statKeyComparison++;
		statBFChange++;
		statAtoYBFChange++;
		if (strcmp(X, P->data) > 0) { P->BF = -1; P = P->RCH; } // adjust BF and move forward
		else { P->BF = +1; P = P->LCH; }
	}
	statAtoYPass++;

	// Now we check the BF at A and see if we just pushed the tree INTO 
	// BALANCE, into an “unacceptable IMBALANCE”, or if it is still
	// “BALANCED ENOUGH”.

	if (A->BF == 0) // Tree WAS completely balanced before the insert.
	{             // The insert pushed it to slight (acceptable) imbalance
		A->BF = d;    // Set the BF to +/- 1 (displacement tells direction)
		statBFChange++;
		return;     // This is close enough to live with, so exit now
	}

	if (A->BF == -1*d) // If the tree had a slight imbalance the OTHER way, 
	{               // then did the insertion throw the tree INTO complete
		A->BF = 0;   // balance? If so, set the BF to zero and we’re done
		statBFChange++;
		return;
	}
	
	// If we end up here, we took neither of the two returns just above, and the tree 
	// is now UNACCEPTABLY IMBALANCED.
	if (d == +1) // this is a left imbalance (left subtree too tall).  
		   // Is it LL or LR?
	{
		if (B->BF == +1) // LL rotation
		{
			// Change the child pointers at A and B to reflect the rotation
			// Adjust the BFs at A & B
			A->LCH = B->RCH; // A's left child now points to B's right
			B->RCH = A; // B's right child now points to A.
			A->BF = 0;
			B->BF = 0;

			statPointerChange += 2;
			statBFChange += 2;
			statLL++;
			statNoRotation--;
		}
		else  // LR Rotation: three cases (structurally the same; BFs vary)
		{
			// Adjust the child pointers of nodes A, B, & C to reflect
			// the new post-rotation structure
			C = B->RCH; // C is B's right child
			CL = C->LCH; // CL and CR are C's left and right children
			CR = C->RCH;

			B->RCH = CL; // B's right child becomes C's original left child
			A->LCH = CR; // A's left child becomes C's original right child
			C->LCH = B; // C's left child becomes B
			C->RCH = A; // C's right child becomes A

			// Set the new BF’s at A and B, based on the BF at C.
			switch (C->BF)
			{
			case 0: A->BF = B->BF = 0; statBFChange--; break; // A, B, & C's BFs all go to 0
			case 1: B->BF = C->BF = 0; A->BF = -1; break; // B & C's BFs go to 0. A's goes to -1
			case -1: A->BF = C->BF = 0; B->BF = 1; break; // A & C's BFs go to 0. B's goes to 1
			default: statBFChange -= 3;
			}

			C->BF = 0; // Regardless, C is now balanced
			B = C;     // B is the root of the now-rebalanced subtree (recycle)
			statPointerChange += 4;
			statBFChange += 3;
			statLR++;
			statNoRotation--;
		} // end of else (LR Rotation)
	} // end of “if (d = +1)”
	else // d=-1.  This is a right imbalance. Is it RR or RL?
	{
		if (B->BF == -1) // RR rotation
		{
			// Change the child pointers at A and B to reflect the rotation
			// Adjust the BFs at A & B
			A->RCH = B->LCH; // A's right child now points to B's left
			B->LCH = A; // B's left child now points to A.
			A->BF = 0;
			B->BF = 0;
			statPointerChange += 2;
			statBFChange += 2;
			statRR++;
			statNoRotation--;
		}
		else  // RL Rotation: three cases (structurally the same; BFs vary)
		{
			// Adjust the child pointers of nodes A, B, & C to reflect
			// the new post-rotation structure
			C = B->LCH; // C is B's left child
			CL = C->LCH; // CL and CR are C's right and left children
			CR = C->RCH; 

			B->LCH = CR; // B's left child becomes C's original right child
			A->RCH = CL; // A's right child becomes C's original left child
			C->RCH = B; // C's right child becomes B
			C->LCH = A; // C's left child becomes A
			statPointerChange += 4;
			// Set the new BF’s at A and B, based on the BF at C.
			switch (C->BF)
			{
			case 0: A->BF = B->BF = 0; statBFChange--; break; // A, B, & C's BFs all go to 0
			case -1: B->BF = C->BF = 0; A->BF = 1; break; // B & C's BFs go to 0. A's goes to -1
			case 1: A->BF = C->BF = 0; B->BF = -1; break; // A & C's BFs go to 0. B's goes to 1
			default: statBFChange -= 3;
			}

			C->BF = 0; // Regardless, C is now balanced
			B = C;     // B is the root of the now-rebalanced subtree (recycle)
			statBFChange += 3;
			statRL++;
			statNoRotation--;
		} // end of else (RL Rotation)
	} // end of "if (d = -1)"

	// Regardless, the subtree rooted at B has been rebalanced, and needs to
	// be the new child of F.  The original subtree of F had root A.
	// did we rebalance the whole tree’s root?
	if (F == NULL) { root = B; return; } // B is the tree’s new root - done

	// The root of what we rebalanced WAS A; now it’s B.  If the subtree we
	// rebalanced was LEFT of F, then B needs to be left of F;
	// if A was RIGHT of F, then B now needs to be right of F.
	//
	if (A == F->LCH) 
	{ 
		F->LCH = B; 
		statPointerChange++;
		return; 
	}
	if (A == F->RCH)
	{
		F->RCH = B;
		statPointerChange++;
		return; 
	}
	cout << "We should never be here\n";
}

// Outputs all the nodes in the AVL tree, their count, key, and balance factor (BF) in comma separated
// values.
void AVL::List()
{
	cout << "AVL tree contains: ";
	if (root != NULL)
		traverse_list(root);
	cout << endl;
}

// A recursive in-order traversal (processes left subtree, then root, then right subtree)
void AVL::traverse_list(node* p)
{
	// If p has a left child, traverse the left subtree.
	if (p->LCH != NULL)
		traverse_list(p->LCH);
	// process the root of the subtree
	cout << p->count << " \'" << p->data << "\'" << " BF=" << p->BF << ",";
	// If p has a right child, traverse the right subtree.
	if (p->RCH != NULL)
		traverse_list(p->RCH);
}

// A recursive in-order traversal (processes left subtree, then root, then right subtree)
// Modifies the two node count parameters to what's found during traversal. 
void AVL::traverse(node* p, int& distinctCount, int& totalCount)
{
	// If p has a left child, traverse the left subtree.
	if (p->LCH != NULL)
		traverse(p->LCH, distinctCount, totalCount);
	// Update the counts.
	distinctCount++;
	totalCount += p->count;
	// If p has a right child, traverse the right subtree.
	if (p->RCH != NULL)
		traverse(p->RCH, distinctCount, totalCount);
}


// Outputs the height of the tree. Here the height is 0 for an empty tree, and 1 for
// a one-node tree.
int AVL::TreeHeight()
{
	int count = 0;
	if (root != NULL) // If the tree isn't empty, get its the height
		count = traverse_height(root);
	return count;
}

// Outputs the AVL trees's statistics in comma-separated values (CSV)
// Gets the height via TreeHeight, node counts via Traverse, and every other metric
// via class-wide variables.
void AVL::DisplayStatistics()
{
	int height = TreeHeight(); // get the tree height found by a traversal
	int distinctCount = 0; 
	int totalCount = 0;
	if (root != NULL)
		traverse(root, distinctCount, totalCount); // get the node counts from a traversal

	// Output everything to the console CSV style
	cout << "AVL STATISTICS:" << endl << endl;
	cout << "Distinct words," << distinctCount << endl;
	cout << "Total words," << totalCount << endl;
	cout << "Height," << height << endl;
	cout << "Pointer changes," << statPointerChange << endl;
	cout << "BF changes," << statBFChange << endl;
	cout << "A to Y BF changes," << statAtoYBFChange << endl;
	cout << "A to Y passes," << statAtoYPass << endl;
	cout << "Key comparisons," << statKeyComparison << endl;
	cout << "LL rotations," << statLL << endl;
	cout << "LR rotations," << statLR << endl;
	cout << "RR rotations," << statRR << endl;
	cout << "RL rotations," << statRL << endl;
	cout << "No rotation needed," << statNoRotation << endl;
}

// Recursively traverse through the tree, comparing the height of the left subtree against
// the right subtree height, return the larger of the two heights.
int AVL::traverse_height(node* p)
{
	int leftSubtreeHeight = 0;
	int rightSubtreeHeight = 0;
	if (p->LCH != NULL) // If left subtree exists, traverse it and get the largest height of the subtree
		leftSubtreeHeight = traverse_height(p->LCH);
	if (p->RCH != NULL) // If right subtree exists, traverse it and get the largest height of the subtree
	{
		rightSubtreeHeight = traverse_height(p->RCH);
	}
	// Return the larger of the two subtree's heights.
	if (leftSubtreeHeight > rightSubtreeHeight)
		return(leftSubtreeHeight + 1);
	else 
		return(rightSubtreeHeight + 1);
}
