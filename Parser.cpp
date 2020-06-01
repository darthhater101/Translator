#include "Parser.h"
#include <map>

std::map<int, std::string> errors = {
	{-1, "\'PROGRAM\' expected!"},
	{-2, "\';\' expected!"},
	{-3, "\'.\' expected!"},
	{-4, "Not identifier!"},
	{-5, "\'BEGIN\' expected!"},
	{-6, "\'END\' expected!"},
	{-7, "\'VAR\' expected!"},
	{-8, "\':\' expected!"},
	{-9, "\'INTEGER\' or \'FLOAT\' expected!"},
	{-10, "\'ENDIF\' expected!"},
	{-11, "\'IF\' expected!"},
	{-12, "\'THEN\' expected!"},
	{-13, "\'=\' expected!"},
	{-14, "Constant or identifier expected!"},
	{-15, "\'ELSE\' expected!"},
	{-16, "\'ENDLOOP\' expected!"}
};

Parser::~Parser()
{
}

void Parser::start()
{
	ERROR_NOT_FOUND = program();
}

bool Parser::program()
{
	tree.add("<program>");
	get_lexem();
	if (current_lexem_code == 301)
	{
		tree.add("PROGRAM");
		tree.step_back();
		get_lexem();
		if (!procedure_identifier())
			return false;
		get_lexem();
		if (current_lexem_code == static_cast<int>(';'))
		{
			tree.add(";");
			tree.step_back();
		}
		else
		{
			generate_error(errors[-2], coord.first, coord.second, current_lexem_code);
			return false;
		}
		if (!block())
			return false;
		get_lexem();
		if (current_lexem_code == static_cast<int>('.'))
		{
			tree.add(".");
			tree.step_back();
		}
		else
		{
			generate_error(errors[-3], coord.first, coord.second, current_lexem_code);
			return false;
		}
	}
	else
	{
		generate_error(errors[-1], coord.first, coord.second, current_lexem_code);
		return false;
	}
	tree.step_back();
	return true;
}

bool Parser::procedure_identifier()
{
	tree.add("<procedure-identifier>");
	if (!identifier())
		return false;
	tree.step_back();
	return true;
}

bool Parser::block()
{
	tree.add("<block>");
	if (!variable_declaration())
		return false;
	if (current_lexem_code == 302)
	{
		tree.add("BEGIN");
		tree.step_back();
		get_lexem();
		if (!statement_list())
			return false;
		if (current_lexem_code == 303)
		{
			tree.add("END");
			tree.step_back();
		}
		else
		{
			generate_error(errors[-6], coord.first, coord.second, current_lexem_code);
			return false;
		}
	}
	else
	{
		generate_error(errors[-5], coord.first, coord.second, current_lexem_code);
		return false;
	}
	tree.step_back();
	return true;
}

bool Parser::variable_declaration()
{
	tree.add("<variable-declaration>");
	get_lexem();
	if (current_lexem_code != 304)
	{
		tree.add("<empty>");
		tree.step_back();
	}
	else
	{
		if (current_lexem_code == 304)
		{
			tree.add("VAR");
			tree.step_back();
			get_lexem();
			if (!declarations_list())
				return false;
			error.str("");
		}
		else
		{
			generate_error(errors[-7], coord.first, coord.second, current_lexem_code);
			return false;
		}
	}
	tree.step_back();
	return true;
}

bool Parser::declarations_list()
{
	tree.add("<declarations-list>");
	bool can_be_decl_error = false;
	if (declaration(can_be_decl_error))
	{
		get_lexem();
		bool b = declarations_list();
		tree.step_back();
		return b;
	}
	else
	{
		if (can_be_decl_error)
			tree.step_back();
		tree.step_back();
		tree.delete_childs();
		tree.add("<empty>");
		tree.step_back();
		tree.step_back();
		return can_be_decl_error;
	}
}

bool Parser::declaration(bool& err)
{
	tree.add("<declaration>");
	if (variable_identifier())
	{
		get_lexem();
		if (current_lexem_code == static_cast<int>(':'))
		{
			tree.add(":");
			tree.step_back();
			get_lexem();
			if (attribute())
			{
				get_lexem();
				if (current_lexem_code == static_cast<int>(';'))
				{
					tree.add(";");
					tree.step_back();
				}
				else
				{
					generate_error(errors[-2], coord.first, coord.second, current_lexem_code);
					return false;
				}
			}
			else
				return false;
		}
		else
		{
			generate_error(errors[-8], coord.first, coord.second, current_lexem_code);
			return false;
		}
		tree.step_back();
		return true;
	}
	tree.step_back();
	err = true;
	return false;
}

bool Parser::variable_identifier()
{
	tree.add("<variable-identifier>");
	if (!identifier())
		return false;
	tree.step_back();
	return true;
}

bool Parser::attribute()
{
	tree.add("<attribute>");
	if (current_lexem_code != tables->get_keywords("INTEGER")
		&& current_lexem_code != tables->get_keywords("FLOAT"))
	{
		generate_error(errors[-9], coord.first, coord.second, current_lexem_code);
		tree.step_back();
		return false;
	}
	else
	{
		tree.add(tables->get_keyword_string(current_lexem_code));
		tree.step_back();
	}
	tree.step_back();
	return true;
}

bool Parser::statement_list()
{
	tree.add("<statement-list>");
	bool can_be_stat_error = false;

	if (statement(can_be_stat_error))
	{
		get_lexem();
		bool b = statement_list();
		tree.step_back();
		return b;
	}
	else
	{
		if (can_be_stat_error)
			tree.step_back();
		tree.step_back();
		tree.delete_childs();
		tree.add("<empty>");
		tree.step_back();
		tree.step_back();
		return can_be_stat_error;
	}
}

bool Parser::statement(bool& err)
{
	tree.add("<statement>");
	if (condition_statement(err))
	{
		if (current_lexem_code == 308)
		{
			tree.add("ENDIF");
			tree.step_back();
			get_lexem();
			if (current_lexem_code == static_cast<int>(';'))
			{
				tree.add(";");
				tree.step_back();
			}
			else
			{
				generate_error(errors[-2], coord.first, coord.second, current_lexem_code);
				return false;
			}
		}
		else
		{
			generate_error(errors[-10], coord.first, coord.second, current_lexem_code);
			return false;
		}
		tree.step_back();
		return true;
	}
	return false;
}

bool Parser::condition_statement(bool& err)
{
	tree.add("<condition-statement>");
	if (!incomplete_condition_statement(err))
		return false;
	if (!alternative_part())
		return false;
	tree.step_back();
	return true;
}

bool Parser::incomplete_condition_statement(bool& err)
{
	tree.add("<incomplete-condition-statement>");
	if (current_lexem_code == 307)
	{
		tree.add("IF");
		tree.step_back();
		get_lexem();
		if (!conditional_expression())
			return false;
		get_lexem();
		if (current_lexem_code == 309)
		{
			tree.add("THEN");
			tree.step_back();
			get_lexem();
			if (!statement_list())
			{
				tree.step_back();
				return false;
			}
		}
		else
		{
			generate_error(errors[-12], coord.first, coord.second, current_lexem_code);
			return false;
		}
		tree.step_back();
		return true;
	}
	tree.step_back();
	err = true;
	return false;
}

bool Parser::conditional_expression()
{
	tree.add("<conditional-expression>");
	if (!expression())
		return false;
	get_lexem();
	if (current_lexem_code == static_cast<int>('='))
	{
		tree.add("=");
		tree.step_back();
	}
	else
	{
		generate_error(errors[-13], coord.first, coord.second, current_lexem_code);
		return false;
	}
	get_lexem();
	if (!expression())
		return false;
	tree.step_back();
	return true;
}

bool Parser::expression()
{
	tree.add("<expression>");
	if (!variable_identifier())
	{
		tree.step_back();
		tree.step_back();
		tree.delete_childs();
		error.str("");
		if (!unsigned_integer())
		{
			return false;
		}
	}
	tree.step_back();
	return true;
}

bool Parser::unsigned_integer()
{
	tree.add("<unsigned-integer>");
	if (!tables->is_constant(current_lexem_code))
	{
		generate_error(errors[-14], coord.first, coord.second, current_lexem_code);
		return false;
	}
	else
	{
		tree.add(tables->get_constant_string(current_lexem_code));
		tree.step_back();
	}
	tree.step_back();
	return true;
}

bool Parser::alternative_part()
{
	tree.add("<alternative-part>");
	if (current_lexem_code != 310)
	{
		tree.add("<empty>");
		tree.step_back();
	}
	else
	{
		if (current_lexem_code == 310)
		{
			tree.add("ELSE");
			tree.step_back();
			get_lexem();
			if (!statement_list())
				return false;
		}
		else
		{
			generate_error(errors[-15], coord.first, coord.second, current_lexem_code);
			return false;
		}
	}
	tree.step_back();
	return true;
}

bool Parser::identifier()
{
	tree.add("<identifier>");
	if (!tables->is_identifier(current_lexem_code))
	{
		generate_error(errors[-4], coord.first, coord.second, current_lexem_code);
		return false;
	}
	else
	{
		tree.add(tables->get_identifier_string(current_lexem_code));
		tree.step_back();
	}
	tree.step_back();
	return true;
}

void Parser::get_lexem()
{
	if (current_position < lexer_result.size())
	{
		current_lexem_code = lexer_result.at(current_position).code;
		coord.first = lexer_result.at(current_position).row;
		coord.second = lexer_result.at(current_position).column;
		current_position++;
	}
}

void Parser::generate_error(std::string expected, int line, int column, int found)
{
	std::string found_str;
	if (tables->is_constant(found))
	{
		found_str = tables->get_constant_string(found);
	}
	else if (tables->is_identifier(found))
	{
		found_str = tables->get_identifier_string(found);
	}
	else if (tables->is_keyword(found))
	{
		found_str = tables->get_keyword_string(found);
	}
	else if (tables->is_separator(found))
	{
		found_str = tables->get_separator_string(found);
	}
	error << "line: " + std::to_string(line) << ", column: " + std::to_string(column) << ": " << expected << ", but \'" << found_str << "\' found\n";
}

