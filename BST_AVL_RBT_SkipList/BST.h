#pragma once
#include <string>

using namespace std;

class BST
{
public:
	BST();
	~BST();

	void Insert(const char* word);
	void List();
	int TreeHeight();
	void DisplayStatistics();
private:
	int statKeyComparison = 0;
	int statPointerChange = 0;

	struct node
	{
		char data[50] = { 0 };
		int count = 1;
		node* LCH = NULL;
		node* RCH = NULL;

		node(const char* word)
		{
			strcpy_s(data, word);
		}
	};

	node* root = NULL;

	void traverse(node* root, int& distinctCount, int& totalCount);
	void traverse_list(node* p, string& list);
	void traverseDelete(node* p);
	int traverse_height(node* p);
};