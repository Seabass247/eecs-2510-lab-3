#include <iostream>
#include "AVL.h"
#include "BST.h"
#include "RBT.h"
#include "SkipList.h"
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

	/*
	RBT* RBT_T = new RBT();
	RBT_T->Insert("E");
	RBT_T->Insert("D");
	RBT_T->Insert("D");
	RBT_T->Insert("D");
	RBT_T->Insert("C");
	RBT_T->Insert("A");
	RBT_T->Insert("B");
	RBT_T->List();
	RBT_T->Height();
	*/

	SkipList* SL = new SkipList();
	SL->Insert("A");
	SL->Insert("A");
	SL->Insert("B");
	SL->Insert("C");
	SL->Insert("D");
	SL->Insert("E");
	SL->Insert("F");
	SL->Insert("G");
	SL->Insert("H");
	SL->Insert("I");
	SL->Insert("J");
	SL->Insert("K");
	SL->Insert("L");
	SL->Insert("M");
	SL->Insert("N");
	SL->Insert("O");
	SL->Insert("O");
	SL->Insert("P");
	SL->Insert("Q");
	SL->Insert("R");
	SL->Insert("S");
	SL->Insert("T");
	SL->Insert("U");
	SL->Insert("V");
	SL->Insert("V");
	SL->Insert("V");
	SL->Insert("V");
	SL->List();
	SL->Height();
}