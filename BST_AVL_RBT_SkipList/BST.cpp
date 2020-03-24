#include "BST.h"
// Sebastian Hamel
// EECS 2510 Non-Linear Data Structures
// 02/23/2020

#include <iostream>
#include "BST.h"

using namespace std;

// The BST's constructor. Configures a new BST where the root is null;
// the tree is empty by default.
BST::BST()
{
	root = NULL;
}

// The BST's destructor.  Called when the variable goes out of scope or is deleted
// via the "delete" keyword.  Delete every node in the tree if the tree is not empty.
BST::~BST()
{
	// If the tree has a root node (it is not empty), delete the root and every
	// child and descendent of the root.  Otherwise, the root is empty, so do nothing.
	if (root != NULL)
	{
		traverseDelete(root);
	}
}

// Makes a post-order recursive traversal through the tree (processing the root after the children)
// and deletes possible left child, right child, then root node recursively for every
// node in the tree, from leaves to root.
void BST::traverseDelete(node* p)
{
	if (p->left != NULL)
		traverseDelete(p->left);
	if (p->right != NULL)
		traverseDelete(p->right);
	delete p;
}


// Inserts a new value into the BST. If the value is already in the tree,
// increment the count of the appropriate node by 1.  Otherwise initialize
// a new empty node (the new node to be inserted is referred to as z here), 
// set its key to <word>. Make z either the root or child of a node which does
// not have at least one child.
void BST::insert(string word)
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
	while (x != NULL)
	{
		// y lags one step behind x, so that if x is null and the loop
		// has exited, y refers to the very last valid node.
		y = x;

		// If the current node's key equals the word of the value we're
		// trying to insert, just increment the count of the word and
		// exit this function.
		if (x->word == word) {
			// Output the nodes word and its count. 
			cout << word << " " << ++x->count << "\n";
			return;
		}
		// Move left or right in the tree depending on if the value we're trying
		// to insert is greater (go right) or less than (go left) the value we're at.
		if (word < x->word)
		{
			x = x->left;
		}
		else
		{
			x = x->right;
		}
	}
	// The loop has exited. x was null, thus we followed a NULL pointer, and y is the leaf.
	// Node z refers to the new node we're inserting into the tree. 
	node* z = new node();
	// z's initialized as a new default node with no word. Set z's key to the word
	// we're inserting.
	z->word = word;

	// If y doesnt exist, it must be that the tree is still empty, therefore we
	// should insert the new value in an empty tree by setting root (the first node)
	// to z.
	if (y == NULL)
	{
		root = z;
	}
	// Otherwise, the tree has at least one node y in it. Make new node z a child of y, 
	// which implies setting z's parent to y as well. 
	// Make z the left child if its word is less than its parents word.
	else if (word < y->word)
	{
		z->parent = y;
		y->left = z;
	}
	// Otherwise it's greater and should be the right child.
	else {
		z->parent = y;
		y->right = z;
	}
	// Output the newly inserted node's word and its count.
	cout << word << " " << z->count << "\n";
}


// A helper function that returns the node of the minimum value
// under node p.
BST::node* BST::minimum(node* p)
{
	// Keep following a left pointer down the tree if it is not NULL. 
	while (p->left != NULL)
	{
		p = p->left;
	}
	// We followed a null left pointer, therefore node p is the minimum.
	return p;
}

// A helper function that returns the node of the maximum value
// under node p.
BST::node* BST::maximum(node* p)
{
	// Keep following a right pointer down the tree if it is not NULL. 
	while (p->right != NULL)
	{
		p = p->right;
	}
	// We followed a null right pointer, therefore node p is the maximum.
	return p;
}

// A helper function the returns the node of the successor to node p.
// The successor is either the minimum of the right child of p if p
// has a right child, otherwise the successor is somewhere higher in
// the tree, where it is found by following the parent, grandparent, etc..
// of p until we followed a left link.
BST::node* BST::successor(node* p)
{
	// If p has a right child, the successor is the minimum of the right
	// child's tree.
	if (p->right != NULL)
	{
		return minimum(p->right);
	}
	// y is an ancestor of p, where p is in y's right subtree.
	node* y = p->parent;
	// Keep going up the tree until we followed a left link.
	while (y != NULL && p == y->right)
	{
		// p lags behind y.
		p = y;
		// go up in the tree.
		y = y->parent;
	}
	// We must have followed a left link and y is therefore the successor, or y is NULL.
	return y;
}

// A helper function the returns the node of the predecessor to node p.
// The predecessor is either the maximum of the left child of p if p
// has a left child, otherwise the predecessor is somewhere higher in
// the tree, where it is found by following the parent, grandparent, etc..
// of p until we followed a right link.
BST::node* BST::predecessor(node* p)
{
	// If p has a left child, the predecessor is the maximum of the left
	// child's tree.
	if (p->left != NULL)
	{
		return maximum(p->left);
	}
	// y is an ancestor of p, where p is in y's left subtree.
	node* y = p->parent;
	// Keep going up the tree until we followed a right link.
	while (y != NULL && p == y->left)
	{
		// p lags behind y.
		p = y;
		// go up in the tree.
		y = y->parent;
	}
	// We must have followed a right link and y is therefore the successor, or y is NULL.
	return y;
}

// Does a in-order traversal through the tree, listing all of the string in the tree in 
// ascending order. Each element in the tree is enumerated from 1 to n, where n is the
// total number of elements in the tree in the form of a label, which appears as 
// '(n) ' followed by string key and its count and a comma to separate it from the
// next element.
void BST::list()
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
		traverse(root, list);

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
// appending comma-separated values to the list string parameter.
void BST::traverse(node* p, string& list)
{
	// If p has a left child, traverse the left subtree.
	if (p->left != NULL)
		traverse(p->left, list);
	// Append '<word> <count>,' to the list string.
	list = list + p->word + " " + to_string(p->count) + ",";
	// If p has a right child, traverse the right subtree.
	if (p->right != NULL)
		traverse(p->right, list);
}

// A helper function which returns the node associated with the key word, 
// and if no node is found, or the tree is empty, returns NULL.
BST::node* BST::find(string word)
{
	//
	node* currentNode = root;
	// Follow the left and right child pointers depending on whether
	// the current word is greater than or less than <word>.
	while (currentNode != NULL)
	{
		// If the current node's key is equal to <word>, we've found
		// the node we're looking for, return it.
		if (currentNode->word == word)
		{
			return currentNode;
		}
		// If the word we're looking for is less than that of the key
		// we're at, go left.
		if (word < currentNode->word)
		{
			currentNode = currentNode->left;
		}
		// Otherwise it's greater, go right.
		else
		{
			currentNode = currentNode->right;
		}
	}
	return NULL;
}

// Output the strings of the keys contained in the left and right
// child nodes of the node containing string word.  If said node
// is not in the treem output a blank line.
void BST::child(string word)
{
	node* p;
	if ((p = find(word)) != NULL)
	{
		// Declare left and right as the pointers to the left
		// child and right child for convenience.
		node* left = p->left;
		node* right = p->right;

		// By default, the words are "NULL" unless there is an
		// actual string key to override it.
		string leftWord = "NULL";
		string rightWord = "NULL";

		// If there is a left child, override "NULL" to its key.
		if (left != NULL)
		{
			leftWord = left->word;
		}
		// If there is a right child, override "NULL" to its key.
		if (right != NULL)
		{
			rightWord = right->word;
		}

		// Output the string of the left child's key and the right child's key.
		cout << leftWord << ", " << rightWord << "\n";

	}
	// There is no node associated with string word in the tree.
	else
	{
		// Output a blank line.
		cout << "\n";
	}
}

// Outputs the parent of the node associated by string word. If said node
// is not in the tree, output a blank line.  Otherwise, if the pointer to 
// the parent is not NULL, p has a parent (here is it referred to as 
// pParent), so output the parent's key.
void BST::parent(string word)
{
	node* p;
	node* pParent;

	// If the node is in the tree and thus if its parent exists, output the parents key.
	if ((p = find(word)) != NULL && (pParent = p->parent) != NULL)
	{
		cout << pParent->word << "\n";
	}
	// Either node is not in the tree or it is, but its parent is NULL, either way output
	// a blank line.
	else
	{
		// Output a blank line.
		cout << "\n";
	}
}