#include "Syntaxer.h"
#include <map>

std::map<int, std::string> errors = {
	{-1, "\'PROGRAM\' expected!\n"},
	{-2, "\';\' expected!\n"},
	{-3, "\'.\' expected!\n"},
	{-4, "Not identifier!\n"},
	{-5, "\'BEGIN\' expected!\n"},
	{-6, "\'END\' expected!\n"},
	{-7, "\'VAR\' expected!\n"},
	{-8, "\':\' expected!\n"},
	{-9, "\'INTEGER\' or \'FLOAT\' expected!\n"},
	{-10, "\'ENDIF\' expected!\n"},
	{-11, "\'IF\' expected!\n"},
	{-12, "\'THEN\' expected!\n"},
	{-13, "\'=\' expected!\n"},
	{-14, "Constant expected!\n"},
	{-15, "\'ELSE\' expected!\n"}
};

Syntaxer::~Syntaxer()
{
}

void Syntaxer::start()
{
	ERROR_NOT_FOUND = program();
}

bool Syntaxer::program()
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
			error << errors[-2];
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
			error << errors[-3];
			return false;
		}
	}
	else
	{
		error << errors[-1];
		return false;
	}
	tree.step_back();
	return true;
}

bool Syntaxer::procedure_identifier()
{
	tree.add("<procedure-identifier>");
	if (!identifier())
		return false;
	tree.step_back();
	return true;
}

bool Syntaxer::block()
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
		//get_lexem();
		if (current_lexem_code == 303)
		{
			tree.add("END");
			tree.step_back();
		}
		else
		{
			error << errors[-6];
			return false;
		}
	}
	else
	{
		error << errors[-5];
		return false;
	}
	tree.step_back();
	return true;
}

bool Syntaxer::variable_declaration()
{
	tree.add("<variable-declaration>");
	get_lexem();
	if (current_lexem_code == 302)
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
		}
		else
		{
			error << errors[-7];
			return false;
		}
	}
	tree.step_back();
	return true;
}

bool Syntaxer::declarations_list()
{
	tree.add("<declarations-list>");
	if (current_lexem_code == 302)
	{
		tree.add("<empty>");
		tree.step_back();
	}
	else
	{
		if (!declaration())
			return false;
		get_lexem();
		if (!declarations_list())
			return false;
	}
	tree.step_back();
	return true;
}

bool Syntaxer::declaration()
{
	tree.add("<declaration>");
	if (!variable_identifier())
		return false;
	get_lexem();
	if (current_lexem_code == static_cast<int>(':'))
	{
		tree.add(":");
		tree.step_back();
	}
	else
	{
		error << errors[-8];
		return false;
	}
	get_lexem();
	if (!attribute())
		return false;
	get_lexem();
	if (current_lexem_code == static_cast<int>(';'))
	{
		tree.add(";");
		tree.step_back();
	}
	else
	{
		error << errors[-2];
		return false;
	}
	tree.step_back();
	return true;
}

bool Syntaxer::variable_identifier()
{
	tree.add("<variable-identifier>");
	if (!identifier())
		return false;
	tree.step_back();
	return true;
}

bool Syntaxer::attribute()
{
	tree.add("<attribute>");
	if (current_lexem_code != tables->get_keywords("INTEGER")
		&& current_lexem_code != tables->get_keywords("FLOAT"))
	{
		error << errors[-9];
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

bool Syntaxer::statement_list()
{
	tree.add("<statement-list>");
	if (current_lexem_code == 303 || current_lexem_code == 310 || current_lexem_code == 308)
	{
		tree.add("<empty>");
		tree.step_back();
	}
	else
	{
		if (!statement())
			return false;
		get_lexem();
		if (!statement_list())
			return false;
	}
	tree.step_back();
	return true;
}

bool Syntaxer::statement()
{
	tree.add("<statement>");
	if (!condition_statement())
		return false;
	//get_lexem();
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
			error << errors[-2];
			return false;
		}
	}
	else
	{
		error << errors[-10];
		return false;
	}
	tree.step_back();
	return true;
}

bool Syntaxer::condition_statement()
{
	tree.add("<condition-statement>");
	if (!incomplete_condition_statement())
		return false;
	//get_lexem();
	if (!alternative_part())
		return false;
	tree.step_back();
	return true;
}

bool Syntaxer::incomplete_condition_statement()
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
				return false;
		}
		else
		{
			error << errors[-12];
			return false;
		}
	}
	else
	{
		error << errors[-11];
		return false;
	}
	tree.step_back();
	return true;
}

bool Syntaxer::conditional_expression()
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
		error << errors[-13];
		return false;
	}
	get_lexem();
	if (!expression())
		return false;
	tree.step_back();
	return true;
}

bool Syntaxer::expression() // error here, need to be fixed
{
	tree.add("<expression>");
	if (!variable_identifier())
	{
		if (!unsigned_integer())
		{
			return false;
		}
	}
	tree.step_back();
	return true;
}

bool Syntaxer::unsigned_integer()
{
	tree.add("<unsigned-integer>");
	if (!tables->is_constant(current_lexem_code))
	{
		error << errors[-14];
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

bool Syntaxer::alternative_part()
{
	tree.add("<alternative-part>");
	if (current_lexem_code == 308)
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
			error << errors[-15];
			return false;
		}
	}
	tree.step_back();
	return true;
}

bool Syntaxer::identifier()
{
	tree.add("<identifier>");
	if (!tables->is_identifier(current_lexem_code))
	{
		error << errors[-4];
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

void Syntaxer::get_lexem()
{
	if (current_position < lexer->get_result().size())
	{
		current_lexem_code = lexer->get_result().at(current_position).code;
		current_position++;
	}
}