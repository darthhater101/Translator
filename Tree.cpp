#include "Tree.h"


void Tree::add(std::string value, int line, int column)
{
	std::shared_ptr<node_s> temp = std::make_shared<node_s>();
	temp->value = value;
	temp->parent = this->current;
	temp->line = line;
	temp->column = column;
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

void Tree::delete_childs()
{
	current->childs.clear();
}

void Tree::write_to_file(std::ostream& stream)
{
	if (root != nullptr)
		write_to_file(root, stream, "-");
}

void Tree::print(std::shared_ptr<node_s>& node, std::string sep)
{
	std::cout << sep.length() << sep << node->value << std::endl;
	if (node->childs.empty())
		return;
	for (auto& it : node->childs)
	{
		print(it, sep + "--");
	}
}

void Tree::write_to_file(std::shared_ptr<node_s>& node, std::ostream& stream, std::string sep)
{
	stream << sep.length() << sep << node->value << std::endl;
	if (node->childs.empty())
		return;
	for (auto& it : node->childs)
	{
		write_to_file(it, stream, sep + "--");
	}
}