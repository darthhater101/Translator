#pragma once
#include "Tree.h"
#include "InformationTables.h"
#include <memory>
#include <vector>
#include <stack>

class Generator
{
private:
	Tree tree;
	std::shared_ptr<InformationTables> tables;
	std::vector<std::string> identifiers;
	std::string program_identifier;
	void traversal(std::shared_ptr<struct Tree::node>& node);
	int label_index = -1;
	std::string program;
public:
	Generator() {}
	Generator(const Tree& tree, const std::shared_ptr<InformationTables>& tables) : tree(tree), tables(tables) {}
	void traversal();
	void print() { std::cout << program; }
};

