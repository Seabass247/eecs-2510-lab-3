#include "AVL.h"

void AVL::insert(string X)
{
	node* Y;            // The new node we insert (z in the previous code)
	node* A, * B; // A will be the last parent above Y with a BF of ±1 (before the insert)
	node* F; // F is A’s parent (F lags one step behind A)
	node* P; // P will be used to scan through the tree until it falls off bottom. 
	node *Q; // Q lags behind P, so it will be the parent of Y when P falls off.
	node* C, * CL, * CR;
	int  d;             // displacement; Used to adjust BFs

	if (root == NULL)   // Empty tree? Make a root node and exit!
	{
		Y = new node;    // make and fill a node
		Y->data = X;
		Y->LCH = Y->RCH = NULL;  // leaf --> no children
		Y->BF = 0;     // it is, by definition, balanced
		root = Y;        // root was NULL, so Y is new root
		return;          // This was the trivial case
	}

	//
	// Locate insertion point for X.
	//
	F = Q = NULL; // F and Q lag, so they start NULL
	A = P = root; // A and P start at the root

	while (P != NULL) // search tree for insertion point
	{
		if (X == P->data) return;  // ALREADY HERE!
		if (P->BF != 0)   // remember the last place we saw
		{
			A = P; F = Q;
		}  // a non-zero BF (and its parent)
		Q = P;                               // Bring Q up to where P is
		P = (X < P->data) ? P->LCH : P->RCH; // and then advance P (based on BST rule to go L or R).
	}

	//
	// At this point, P is NULL, but Q points at the last node where X
	// belongs (either as Q’s LCH or RCH, and Q points at an existing leaf)
	//
	Y = new node;   // Make a new node
	Y->data = X;    // Put our data (X) in it
	Y->LCH = NULL; // New nodes are always inserted...
	Y->RCH = NULL; // ...as leaves (i.e., no children)
	Y->BF = 0;    // Leaves are always balanced by definition


	// Will Y be Q's new left or right child?
	if (X < Q->data) Q->LCH = Y;
	else Q->RCH = Y;

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

	if (X > A->data) { B = P = A->RCH; d = -1; } // Which way is the displacement (d)
	else { B = P = A->LCH; d = +1; } // B is identified as A’s child

	while (P != Y)  // P is now one node below A.  Adjust from here to the
	{               // insertion point.  Don’t do anything to new node (Y)
		if (X > P->data) { P->BF = -1; P = P->RCH; } // adjust BF and move forward
		else { P->BF = +1; P = P->LCH; }
	}

	// Now we check the BF at A and see if we just pushed the tree INTO 
	// BALANCE, into an “unacceptable IMBALANCE”, or if it is still
	// “BALANCED ENOUGH”.

	if (A->BF == 0) // Tree WAS completely balanced before the insert.
	{             // The insert pushed it to slight (acceptable) imbalance
		A->BF = d;    // Set the BF to +/- 1 (displacement tells direction)
		return;     // This is close enough to live with, so exit now
	}

	if (A->BF == -d) // If the tree had a slight imbalance the OTHER way, 
	{               // then did the insertion throw the tree INTO complete
		A->BF = 0;   // balance? If so, set the BF to zero and we’re done
		return;      //
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
			B->RCH = A;
			A->LCH = NULL;
			F->LCH = B;
		}
		else  // LR Rotation: three cases (structurally the same; BFs vary)
		{
			// Adjust the child pointers of nodes A, B, & C to reflect
			// the new post-rotation structure
			C = B->RCH; // C is B's right child
			CL = C->LCH; // CL and CR are C's left and right children
			CR = C->RCH; // See Schematic (2) and (3) 
			
			C->LCH = B; // B becomes C's left child
			C->RCH = A; // A becomes C's right child
			B->RCH = CL; // B's right child becomes C's original left child
			A->LCH = CR; // A's left child becomes C's original right child

			switch (C->BF)
			{
				// Set the new BF’s at A and B, based on the BF at C.
				case 0: A->BF = B->BF = C->BF = 0; break; // A, B, & C's BFs all go to 0
				case 1: B->BF = C->BF = 0; A->BF = -1; break; // B & C's BFs go to 0. A's goes to -1
				case -1: A->BF = C->BF = 0; B->BF = 1; break; // A & C's BFs go to 0. B's goes to 1
			}

			C->BF = 0; // Regardless, C is now balanced
			B = C;     // B is the root of the now-rebalanced subtree (recycle)
		} // end of else (LR Rotation)
	} // end of “if (d = +1)”




}
