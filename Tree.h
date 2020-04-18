#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <ostream>
#include <optional>

class Tree
{
private:
	struct node
	{
		std::string value;
		std::vector<std::shared_ptr<node>> childs;
		std::shared_ptr<node> parent;
		int line;
		int column;
	};

	using node_s = struct node;

	std::shared_ptr<node_s> root;
	std::shared_ptr<node_s> current;

	void print(std::shared_ptr<node_s>&, std::string sep);
	void write_to_file(std::shared_ptr<node_s>& node, std::ostream& stream, std::string sep);

public:
	Tree() 
	{
		root = std::make_shared<node_s>();
		root->value = "<signal-program>";
		root->parent = nullptr;
		current = root;
	}
	void add(std::string value, int line = -1, int column = -1);
	void print();
	void step_back();
	void delete_childs();
	void write_to_file(std::ostream& stream);

	std::shared_ptr<node_s> ger_root() { return root; }

	friend class Generator;
};

