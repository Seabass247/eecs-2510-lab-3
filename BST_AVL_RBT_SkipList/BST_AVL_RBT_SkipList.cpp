#include <iostream>
#include "AVL.h"
#include "BST.h"
#include "RBT.h"

int main()
{
	/*
	AVL* AVL_T = new AVL();
	AVL_T->Insert("A");
	AVL_T->Insert("B");
	AVL_T->Insert("C");
	AVL_T->Insert("D");
	AVL_T->Insert("E");
	AVL_T->List();
	AVL_T->Height();
	BST* BST_T = new BST();
	BST_T->Insert("A");
	BST_T->Insert("B");
	BST_T->Insert("C");
	BST_T->Insert("D");
	BST_T->Insert("E");
	BST_T->List();
	BST_T->Height();
	*/

	RBT* RBT_T = new RBT();
	RBT_T->Insert("B");
	RBT_T->Insert("A");
	RBT_T->Insert("C");
	RBT_T->Insert("D");
	RBT_T->Insert("D");
	RBT_T->Insert("D");
	RBT_T->Insert("E");
	RBT_T->List();
	RBT_T->Height();
}