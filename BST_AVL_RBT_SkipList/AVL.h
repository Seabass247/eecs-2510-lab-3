#pragma once
#include <string>

using namespace std;

class AVL
{
public:
	AVL();

	void Insert(const char* word);
	void List();
	int TreeHeight();
	void DisplayStatistics();
private:
	int statKeyComparison = 0;
	int statPointerChange = 0;
	int statBFChange = 0;
	int statAtoYBFChange = 0;
	int statLL = 0;
	int statLR = 0;
	int statRR = 0;
	int statRL = 0;
	int statNoRotation = 0;

	struct node
	{
		const char* data;
		int count = 1;
		int BF = 0;
		node* LCH = NULL;
		node* RCH = NULL;
	};

	node* root = NULL;
	void traverse_list(node* p);
	void traverse(node* p, int& distinctCount, int& totalCount);
	int traverse_height(node* p);
};
