#pragma once
#include "Lexer.h"
#include "InformationTables.h"
#include "Tree.h"

class Parser
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
	bool statement_list();
	bool statement();
	bool condition_statement();
	bool incomplete_condition_statement();
	bool conditional_expression();
	bool expression();
	bool unsigned_integer();
	bool alternative_part();

	bool identifier();

	void get_lexem();

	std::vector<struct Lexer::lexem> lexer_result;
	std::shared_ptr<InformationTables> tables;
	Tree tree;
	int current_lexem_code = 0;
	int current_position = 0;
	std::pair<int, int> coord;
	std::stringstream error;
	bool ERROR_NOT_FOUND = true;

public:
	Parser() noexcept {}
	Parser(std::vector<struct Lexer::lexem>& lexer_result, 
			 std::shared_ptr<InformationTables>& tables)
	{
		this->lexer_result = lexer_result;
		this->tables = tables;
		error << "Parser error: ";
	}
	~Parser();
	void start();
	bool is_success() { return ERROR_NOT_FOUND; }
	std::string get_error() { return error.str(); }
	Tree get_tree() { return tree; }

};

