#pragma once
#include <iostream>
#include <vector>

class Tree
{
private:
	struct node
	{
		std::string value;
		std::vector<node*> childs;
		node* parent;
	};

	using node_t = struct node;

	node_t* root;
	node_t* current;

	void print(node_t* node, std::string sep);

public:
	Tree()
	{
		root = new node_t;
		root->value = "<signal-program>";
		root->parent = nullptr;
		current = root;
	}
	void add(std::string value);
	void print();
	void step_back();
};

