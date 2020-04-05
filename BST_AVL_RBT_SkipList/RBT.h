// RBT.h
// Sebastian Hamel
// EECS 2510, Spring 2020
// 04/03/2020
// Contains the definitions for the RBT class and its internal node structure
#pragma once
#include <string>

using namespace std;

class RBT
{
public:
	RBT();
	void Insert(const char* word);
	void List();
	void DisplayStatistics();
	int  TreeHeight();
private:
	int statKeyComparison = 0;
	int statChildPointerChange = 0;
	int statParentPointerChange = 0;
	int statPointerChange = 0;
	int statRecoloring = 0;
	int statleftRotation = 0;
	int statrightRotation = 0;
	int statCase1 = 0;
	int statCase2 = 0;
	int statCase3 = 0;
	int statnoFixup = 0;

	enum class Color { red, black };
	struct node
	{
		char data[50] = {0};
		int count = 1;
		node* LCH = NULL;
		node* RCH = NULL;
		node* parent = NULL;
		Color color = Color::black;

		node(const char* word)
		{
			strcpy_s(data, word);
		}
	};

	node* root = NULL;
	node* nil = NULL;

	void insert_fixup(node* z); 
	void left_rotate(node* x);
	void right_rotate(node* x);
	void traverse(node* p, int& distinctNodes, int& totalNodes);
	void traverse_list(node* p);
	int traverse_height(node* p);
};