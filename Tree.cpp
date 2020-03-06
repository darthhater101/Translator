#include "Tree.h"


void Tree::add(std::string value)
{
	node_t* temp = new node_t;
	temp->value = value;
	temp->parent = this->current;
	current->childs.push_back(temp);
	current = temp;
}

void Tree::print()
{
	if (root != nullptr)
		print(root, "-");
}

void Tree::step_back()
{
	current = current->parent;
}

void Tree::print(node_t* node, std::string sep)
{
	std::cout << sep << node->value << std::endl;
	if (node->childs.empty())
		return;
	for (auto& it : node->childs)
	{
		print(it, sep + "--");
	}
}
