#pragma once
#include <string>

using namespace std;

class BST
{
public:
	BST();
	~BST();

	void insert(string word);
	void parent(string word);
	void child(string word);
	void list();

private:
	struct node
	{
		string word;
		int count = 1;
		node* left = NULL;
		node* right = NULL;
		node* parent = NULL;
	};

	node* root = NULL;

	node* minimum(node* p);
	node* maximum(node* p);
	node* successor(node* p);
	node* predecessor(node* p);
	node* find(string word);
	void traverse(node* p, string& list);
	void traverseDelete(node* p);
};