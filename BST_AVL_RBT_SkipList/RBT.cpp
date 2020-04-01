#include <iostream>
#include "RBT.h"

using namespace std;

// RBT's constructor. Initializes the tree's nil node
RBT::RBT()
{
	// Initialize the nil node, containing an empty string for a key,
	// points to itself for children and parent.
	node* nil = new node("");
	nil->color = Color::black;
	nil->parent = nil;
	nil->LCH = nil;
	nil->RCH = nil;
	this->nil = nil;
	statChildPointerChange += 2;
	statParentPointerChange++;
}

// Inserts a new value into the RBT Tree. If the value is already in the tree,
// increment the count of the appropriate node by 1 and return.  Otherwise,
// initialize a new node, find an appropriate place to insert it based on the 
// value of its key, then fix any RBT violations caused by the insertion.
void RBT::Insert(char X[50])
{
	node* P; // P will be used to scan through the tree until it falls off bottom. 
	node* Q; // Q lags behind P, so it will be the parent of Y when P falls off.
	
	//
	// Locate insertion point for X.
	//
	Q = nil; // Q lags behind P so it starts NULL
	P = root; // P starts at the root
	
	while (P != nil && P != NULL) // search tree for insertion point
	{
		int comparisonValue = strcmp(X, P->data);
		statKeyComparison++;
		if (comparisonValue == 0) {
			P->count++;
			return;  // ALREADY HERE!
		}
		Q = P; // Bring Q up to where P is
		P = (comparisonValue < 0) ? P->LCH : P->RCH; // and then advance P (based on BST rule to go L or R).
	}

	if (Q == nil) // Empty tree? Make a new node (R) the root and exit!
	{
		
		node* R = new node(X); // make and fill a node
		R->LCH = R->RCH = nil; // leaf --> children point to nil
		R->parent = nil; // point to the nil node
		statChildPointerChange += 2;
		statParentPointerChange++;
		root = R; // root was NULL, so R is new root
		return; // This was the trivial case
	}
	//
	// At this point, P is nil, but Q points at the last node where X
	// belongs (either as Q’s LCH or RCH, and Q points at an existing leaf)
	//
	node* Z = new node(X);   // Make a new node (Z) to be inserted
	//Z->data = *X;    // Put our data (X) in it
	Z->LCH = nil; // New nodes are always inserted...
	Z->RCH = nil; // ...as leaves, where RBT leaves' children point to nil
	Z->parent = Q; // Y's parent is that last node we found before falling off the tree
	Z->color = Color::red; // Color this node red for now (safe?)
	statChildPointerChange++;
	// Will Y be Q's new left or right child?
	if (strcmp(X, Q->data) < 0) Q->LCH = Z;
	else Q->RCH = Z;
	statKeyComparison++;
	statChildPointerChange++;

	// Take care of any potential rule violations caused by this insertion
	insert_fixup(Z);
}

// Outputs all the nodes in the RBT tree, their count, key, and balance factor (BF) in comma-separated
// values.
void RBT::List()
{
	cout << "RBT tree contains: ";
	if (root != nil && root != NULL)
		traverse_list(root);
	cout << endl;
}

void RBT::DisplayStatistics()
{
	int height = TreeHeight();
	int distinctNodes = 0;
	int totalNodes = 0;
	if (root != NULL)
		traverse(root, distinctNodes, totalNodes);

	cout << "RBT_distinct_items=" << distinctNodes << endl;
	cout << "RBT_total_items=" << totalNodes << endl;
	cout << "RBT_height=" << height << endl;
	cout << "RBT_key_comparisons=" << statKeyComparison << endl;
	cout << "RBT_child_pointer_changes=" << statChildPointerChange << endl;
	cout << "RBT_parent_pointer_changes=" << statParentPointerChange << endl;
	cout << "RBT_recolorings=" << statRecoloring << endl;
	cout << "RBT_left_rotations=" << statleftRotation << endl;
	cout << "RBT_right_rotations=" << statrightRotation << endl;
	cout << "RBT_case_1_problems=" << statCase1 << endl;
	cout << "RBT_case_2_problems=" << statCase2 << endl;
	cout << "RBT_case_3_problems=" << statCase3 << endl;
}

// Outputs the height of the tree. Here the height is 0 for an empty tree, and 1 for
// a one-node tree.
int RBT::TreeHeight()
{
	int count = 0;
	if (root != NULL) // If the tree isn't empty, get its the height
		count = traverse_height(root);
	return count;
}


// Recursively traverse through the tree, comparing the height of the left subtree against
// the right subtree height, return the larger of the two heights.
int RBT::traverse_height(node* p)
{
	int leftSubtreeHeight = 0;
	int rightSubtreeHeight = 0;
	if (p->LCH != nil) // If left subtree exists, traverse it and get the largest height of the subtree
		leftSubtreeHeight = traverse_height(p->LCH);
	if (p->RCH != nil) // If right subtree exists, traverse it and get the largest height of the subtree
	{
		rightSubtreeHeight = traverse_height(p->RCH);
	}
	// Return the larger of the two subtree's heights.
	if (leftSubtreeHeight > rightSubtreeHeight)
		return(leftSubtreeHeight + 1);
	else
		return(rightSubtreeHeight + 1);
}


// Checks for violations of the RBT properties and fixes the tree accordingly.
// The conditions for a violation are based on the node being to passed to this
// function (z) that is a newly inserted node in the tree.
void RBT::insert_fixup(node* z)
{
	while (z->parent->color == Color::red) // The following rules are only enforced where z's parent is red
	{
		if (z->parent == z->parent->parent->LCH) // is z's parent a left child of *its* parent?
		{
			node* y = z->parent->parent->RCH; // let y be z's right uncle
			if (y->color == Color::red)
			{
				z->parent->color = Color::black; // Case 1 (re-color only)
				y->color = Color::black; // Case 1
				z->parent->parent->color = Color::red; // Case 1
				statRecoloring += 3;
				z = z->parent->parent; // Case 1 (z becomes z's grandparent)
				statCase1++;
			}
			else // else its possibly a case 2 problem, which always falls into case 3
				// it's a case 3 problem directly if z is a left child
			{
				if (z == z->parent->RCH) // is z the right of child of its parent?
				{
					z = z->parent; // Case 2
					left_rotate(z); // Case 2
					statCase2++;
				}
				z->parent->color = Color::black; // Case 3
				z->parent->parent->color = Color::red; // Case 3
				statRecoloring += 2;
				right_rotate(z->parent->parent); // Case 3
				statCase3++;
			}
		} else // Symmetric case; is z's parent a *right* child of *its* parent?
		{
			node* y = z->parent->parent->LCH; // let y be z's left uncle
			if (y->color == Color::red)
			{
				z->parent->color = Color::black; // Case 1 (re-color only)
				y->color = Color::black; // Case 1
				z->parent->parent->color = Color::red; // Case 1
				statRecoloring += 3;
				z = z->parent->parent; // Case 1 (z becomes z's grandparent)
				statCase1++;
			}
			else // else its possibly a case 2 problem, which always falls into case 3
				// it's a case 3 problem directly if z is a right child
			{
				if (z == z->parent->LCH) // is z the left of child of its parent?
				{
					z = z->parent; // Case 2
					right_rotate(z); // Case 2
					statCase2++;
				}
				z->parent->color = Color::black; // Case 3
				z->parent->parent->color = Color::red; // Case 3
				statRecoloring += 2;
				left_rotate(z->parent->parent); // Case 3
				statCase3++;
			}
		}
	} // end of cases

	root->color = Color::black; // Takes care of the potential "rule 2" violation
}

// Changes relevant pointers to nodes in the tree to reflect a left rotation around node x.
void RBT::left_rotate(node* x)
{
	node* y = x->RCH; // y points to x's right child
	x->RCH = y->LCH; // x's right subtree becomes y's left
	statChildPointerChange++;
	if (y->LCH != nil)
	{
		y->LCH->parent = x;
		statParentPointerChange++;
	}
	y->parent = x->parent; // link x's parent to y
	statParentPointerChange++;
	if (x->parent == nil)
		root = y;
	else if (x == x->parent->LCH)
	{
		x->parent->LCH = y;
		statChildPointerChange++;
	}
	else 
	{ 
		x->parent->RCH = y;
		statChildPointerChange++;
	}
	y->LCH = x; // put x on y's left
	statChildPointerChange++;
	x->parent = y;
	statParentPointerChange++;
	statleftRotation++;
}

// Changes relevant pointers to nodes in the tree to reflect a right rotation around node x.
void RBT::right_rotate(node* x)
{
	node* y = x->LCH; // y points to x's left child
	x->LCH = y->RCH; // x's left subtree becomes y's right
	statChildPointerChange++;
	if (y->RCH != nil)
	{
		y->RCH->parent = x;
		statParentPointerChange++;
	}
	y->parent = x->parent; // link x's parent to y
	statParentPointerChange++;
	if (x->parent == nil)
		root = y;
	else if (x == x->parent->RCH)
	{
		x->parent->RCH = y;
		statChildPointerChange++;
	}
	else
	{
		x->parent->LCH = y;
		statChildPointerChange++;
	}
	y->RCH = x; // put x on y's right
	statChildPointerChange++;
	x->parent = y;
	statParentPointerChange++;
	statrightRotation++;
}

// A recursive in-order traversal (processes left subtree, then root, then right subtree)
void RBT::traverse(node* p, int& distinctNodes, int& totalNodes)
{
	// If p has a left child, traverse the left subtree.
	if (p->LCH != nil)
		traverse(p->LCH, distinctNodes, totalNodes);
	// process the root of the subtree
	distinctNodes++;
	totalNodes += p->count;
	// If p has a right child, traverse the right subtree.
	if (p->RCH != nil)
		traverse(p->RCH, distinctNodes, totalNodes);
}

// A recursive in-order traversal (processes left subtree, then root, then right subtree)
void RBT::traverse_list(node* p)
{
	// If p has a left child, traverse the left subtree.
	if (p->LCH != nil)
		traverse_list(p->LCH);
	// process the root of the subtree
	cout << p->count << " \'" << p->data << "\'" << " COLOR=" << (int)p->color << ",";
	// If p has a right child, traverse the right subtree.
	if (p->RCH != nil)
		traverse_list(p->RCH);
}