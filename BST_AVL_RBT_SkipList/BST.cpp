// BST.cpp
// Sebastian Hamel
// EECS 2510, Spring 2020
// 04/03/2020
// Contains the implementation of the BST class. Enables the BST to have words inserted and exposes the 
// resulting statistics via DisplayStatistics.
#include "BST.h"
#include <iostream>

using namespace std;

// The BST's constructor. Configures a new BST where the root is null;
// the tree is empty by default.
BST::BST()
{
	root = NULL;
}

// Inserts a new value into the BST. If the value is already in the tree,
// increment the count of the appropriate node by 1.  Otherwise initialize
// a new empty node (the new node to be inserted is referred to as z here), 
// set its key to <word>. Make z either the root or child of a node which does
// not have at least one child.
void BST::Insert(const char* word)
{
	// Node x is the root node and possibly any descendent that has
	// a key matching the word we're inserting.
	node* x = root;
	// Node y is the lagging pointer behind x, such that if x is null
	// (AKA we fell of leaf in our search) y would be the valid leaf node.
	node* y = NULL;

	// Search for a point to insert a new node (setting y to the node which will be the
	// parent of the new node).  If at any point the current node in the search
	// contains a key equal to <word>, output the node's key and count and exit
	// this funciton.
	int comparisonValue;
	while (x != NULL)
	{
		// y lags one step behind x, so that if x is null and the loop
		// has exited, y refers to the very last valid node.
		y = x;

		comparisonValue = strcmp(word, x->data);
		statKeyComparison++;
		// If the current node's key equals the word of the value we're
		// trying to insert, just increment the count of the word and
		// exit this function.
		if (comparisonValue == 0) {
			x->count++;
			return;
		}
		// Move left or right in the tree depending on if the value we're trying
		// to insert is greater (go right) or less than (go left) the value we're at.
		if (comparisonValue < 0)
		{
			x = x->LCH;
		}
		else
		{
			x = x->RCH;
		}
	}
	// The loop has exited. x was null, thus we followed a NULL pointer, and y is the leaf.
	// Node z refers to the new node we're inserting into the tree. 
	// z's initialized as a new default node with no word. Set z's key to the word
	// we're inserting.
	node* z = new node(word);

	// If y doesnt exist, it must be that the tree is still empty, therefore we
	// should insert the new value in an empty tree by setting root (the first node)
	// to z.
	if (y == NULL)
	{
		root = z;
		statPointerChange++;
	}
	// Otherwise, the tree has at least one node y in it. Make new node z a child of y, 
	// which implies setting z's parent to y as well. 
	// Make z the left child if its word is less than its parents word.
	else if (comparisonValue < 0)
	{
		y->LCH = z;
		statPointerChange++;
	}
	// Otherwise it's greater and should be the right child.
	else {
		y->RCH = z;
		statPointerChange++;
	}
}

// Does a in-order traversal through the tree, listing all of the string in the tree in 
// ascending order. Each element in the tree is enumerated from 1 to n, where n is the
// total number of elements in the tree in the form of a label, which appears as 
// '(n) ' followed by string key and its count and a comma to separate it from the
// next element.
void BST::List()
{
	// Output and nicely format the list only if the tree is not empty.  
	if (root != NULL)
	{
		cout << "Set contains: ";
		// Declare an empty list string to be populated by comma-separated values.
		string list;

		// By default we assume the first item is number 1.
		int itemCount = 1;

		// Populate the list string with a list of comma-separated values of items and their
		// respective counts.
		traverse_list(root, list);

		// Parse the comma-separated string list of values into a readable print format,
		// where each item and its count are numbered from 0 to n items in the tree.
		for (int i = 0; i < list.length(); i++)
		{
			// Create the label that will be inserted in front of each item. Looks like ', (n) '
			// where n is the item's number.
			string label = ", (" + to_string(itemCount) + ") ";
			// Replace comma with ', (n) ' where n is the number of the item.
			if (list[i] == ',')
			{
				list.replace(i, 1, label);
				itemCount++; // Increments the item count for the label of the next item, so that it's '(n+1)'
			}
			else if (i == (list.length() - 1)) // At the last character of the list string?
			{
				// Remove the last character (an extraneous comma ',') from the list.
				list = list.substr(0, list.length() - label.length());
				itemCount++; // Increments the item count for the label of the next item, so that it's '(n+1)'
			}
			// Insert the first label at very the beginning of the comma-separated list
			// (This branch is necessary because there is no comma at the beginning of the list to replace,
			// and the label string must be trimmed to '(n) ' from original: ', (n) ').
			else if (i == 0)
			{
				// Trim the label to '(n) ', for the beginning of the list shouldn't start with a comma,
				// and insert the label at the very beginnning of the list string.
				list.insert(0, label.substr(2, label.length()));
				itemCount++; // Increments the item count for the label of the next item, so that it's '(n+1)'
			}
		}

		// Output the list, which is now formatted correctly, on a new line.
		cout << list << "\n";
	}
	// The root is null, therefore the list is empty.
	else
	{
		cout << "Set is empty \n";
	}
}

// A recursive in-order traversal (processes left subtree, then root, then right subtree),
// while adding 1 to the node count for every node and adding p.count to totalCount every time.
void BST::traverse(node* p, int& distinctCount, int& totalCount)
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

// A recursive in-order traversal (processes left subtree, then root, then right subtree),
// appending comma-separated values to the list string parameter.
void BST::traverse_list(node* p, string& list)
{
	// If p has a left child, traverse the left subtree.
	if (p->LCH != NULL)
		traverse_list(p->LCH, list);
	// Append '<word> <count>,' to the list string.
	list = list + p->data + " " + to_string(p->count) + ",";
	// If p has a right child, traverse the right subtree.
	if (p->RCH != NULL)
		traverse_list(p->RCH, list);
}

// Returns the height of the tree. Here the height is 0 for an empty tree, and 1 for
// a one-node tree.
int BST::TreeHeight()
{
	int count = 0;
	if (root != NULL) // If the tree isn't empty, get its the height
		count = traverse_height(root);
	return count;
}

// Outputs the BST's statistics in comma-separated values (CSV)
// Gets the height via TreeHeight, node counts via Traverse, and every other metric
// via class-wide variables.
void BST::DisplayStatistics()
{
	int height = TreeHeight(); // get the tree height found by a traversal
	int distinctNodes = 0; 
	int totalNodes = 0;
	if (root != NULL)
		traverse(root, distinctNodes, totalNodes); // get the node counts from a traversal

	// Output everything to the console CSV style
	cout << "BST Statistics: " << endl << endl;
	cout << "Distinct words," << distinctNodes << endl;
	cout << "Total words," << totalNodes << endl;
	cout << "Height," << height << endl;
	cout << "Child pointer changes," << statPointerChange << endl;
	cout << "Key comparisons," << statKeyComparison << endl;
	
}

// Recursively traverse through the tree, comparing the height of the left subtree against
// the right subtree height, return the larger of the two heights.
int BST::traverse_height(node* p)
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
