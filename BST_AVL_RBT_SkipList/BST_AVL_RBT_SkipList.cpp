#include <iostream>
#include "AVL.h"

int main()
{
	AVL* AVL_T = new AVL();
	AVL_T->Insert("MAY");
	AVL_T->Insert("MAR");
	AVL_T->Insert("NOV");
	AVL_T->Insert("AUG");
	AVL_T->list();
	AVL_T->Height();
}