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
	void Height();
private:
	struct node
	{
		const char* word;
		int count = 1;
		node* LCH = NULL;
		node* RCH = NULL;
		node* parent = NULL;
	};

	node* root = NULL;

	node* find(string word);
	void traverse(node* p, string& list);
	void traverseDelete(node* p);
	int traverse_height(node* p);
};