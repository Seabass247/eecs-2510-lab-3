#pragma once
#include <string>

using namespace std;

class AVL
{
public:
	AVL();

	void Insert(const char* word);
	void list();

private:
	struct node
	{
		const char* data;
		int count = 1;
		int BF = 0;
		node* LCH = NULL;
		node* RCH = NULL;
	};

	node* root = NULL;
	node* find(const char* word);
	void traverse(node* p);
};
