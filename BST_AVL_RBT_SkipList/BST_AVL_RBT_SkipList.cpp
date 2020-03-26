#include <iostream>
#include "AVL.h"
#include "BST.h"

int main()
{
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

}