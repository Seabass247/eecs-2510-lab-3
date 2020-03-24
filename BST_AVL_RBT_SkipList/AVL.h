#pragma once
#include <string>

using namespace std;

class AVL
{
public:
	AVL();
	~AVL();

	void insert(string word);
	void list();

private:
	struct node
	{
		string data;
		int count = 1;
		int BF = 0;
		node* LCH = NULL;
		node* RCH = NULL;
	};

	node* root = NULL;

	node* find(string word);
	void traverse(node* p, string& list);
	void traverseDelete(node* p);
};