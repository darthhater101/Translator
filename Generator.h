#pragma once
#include "Tree.h"
#include "InformationTables.h"
#include <memory>
#include <vector>
#include <stack>
#include <sstream>

class Generator
{
private:
	Tree tree;
	std::shared_ptr<InformationTables> tables;
	std::vector<std::string> identifiers;
	std::string program_identifier;
	int if_index = 1;
	int else_index = 1;
	std::stack<int> if_stack;
	std::stack<int> else_stack;
	std::string program;
	std::stringstream error;
	bool data_exists = false;

	void traversal(const std::shared_ptr<struct Tree::node>& node);
	bool identifier_exists(std::string identifier);
public:
	Generator() {}
	Generator(const Tree& tree, const std::shared_ptr<InformationTables>& tables) : tree(tree), tables(tables) {}
	void traversal();
	std::string get_error() { return error.str(); }
	void print() { std::cout << program; }
	void write_to_file(std::ostream& stream) { stream << program; stream << error.str(); }
};

