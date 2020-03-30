#pragma once
#include <string>

using namespace std;

class RBT
{
public:
	RBT();
	~RBT();

	void Insert(const char* word);
	void List();
	int  TreeHeight();
private:
	enum class Color { red, black };
	struct node
	{
		const char* data = "";
		int count = 1;
		node* LCH = NULL;
		node* RCH = NULL;
		node* parent = NULL;
		Color color = Color::black;
	};

	node* root = NULL;
	node* nil = NULL;

	void insert_fixup(node* z); 
	void left_rotate(node* x);
	void right_rotate(node* x);
	node* find(const char* word);
	void traverse(node* p);
	int traverse_height(node* p);
};