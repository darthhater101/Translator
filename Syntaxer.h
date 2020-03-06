#pragma once
#include "Lexer.h"
#include "InformationTables.h"
#include "Tree.h"

class Syntaxer
{
private:

	bool program();
	bool procedure_identifier();
	bool block();
	bool variable_declaration();
	bool declarations_list();
	bool declaration();
	bool variable_identifier();
	bool attribute();
	bool statemant_list();

	bool identifier();

	void get_lexem();

	Lexer* lexer = nullptr;
	InformationTables* tables = nullptr;
	Tree tree;
	int current_lexem_code = 0;
	int current_position = 0;
	std::stringstream error;
	bool ERROR_NOT_FOUND = true;

public:
	Syntaxer() noexcept {}
	Syntaxer(Lexer* lexer, InformationTables* tables)
	{
		this->lexer = lexer;
		this->tables = tables;
	}
	~Syntaxer();
	void start();
	bool is_success() { return ERROR_NOT_FOUND; }
	std::string get_error() { return error.str(); }
	Tree get_tree() { return tree; }

};

