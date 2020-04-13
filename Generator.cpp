#include "Generator.h"

void Generator::traversal(std::shared_ptr<struct Tree::node>& node)
{
	if (node == nullptr) return;

	else if (node->value == "<program>")
	{
		for (auto& it : node->childs)
		{
			if (it->value == "<procedure-identifier>")
			{
				program_identifier = it->childs[0]->childs[0]->value;
			}
		}
	}
	else if (node->value == "VAR")
	{
		std::cout << "DATA SEGMENT" << std::endl;
	}
	else if (node->value == "<declaration>")
	{
		for (auto& it : node->childs)
		{
			if (it->value == "<variable-identifier>")
			{
				if (std::find(identifiers.begin(), identifiers.end(), it->childs[0]->childs[0]->value) != identifiers.end())
				{
					std::cout << "Generator: such identifier already exist: " + it->childs[0]->childs[0]->value << "\n";
					return;
				}
				std::cout << it->childs[0]->childs[0]->value << " ";
			}
			if (it->value == "<attribute>")
			{
				if(it->childs[0]->value == "INTEGER")
					std::cout << "DW  ?\n";
				if (it->childs[0]->value == "FLOAT")
					std::cout << "DW  ?\n";
			}
		}
	}
	else if (node->value == "<declarations-list>")
	{
		if (node->childs[0]->value == "<empty>")
		{
			std::cout << "DATA ENDS\n";
		}
	}
	else if (node->value == "BEGIN")
	{
		std::cout << "CODE SEGMENT\n\tASSUME CS:CODE, DS:DATA\n";
		std::cout << program_identifier << ":\n";
	}
	else if (node->value == "END")
	{
		std::cout << "MOV AH, 4Ch\nINT 21h\nCODE ENDS\nEND " << program_identifier;
	}


	if (node->childs.empty()) return;
	for (auto& it : node->childs)
	{
		traversal(it);
	}
}

void Generator::traversal()
{
	this->traversal(tree.root);
}
