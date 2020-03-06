#include "Lexer.h"
#include "InformationTables.h"
#include "Syntaxer.h"
#include <iostream>

int main()
{
	InformationTables* tables = new InformationTables();
	Lexer lexer("Test.txt", tables);
	lexer.start();
	
	for (const auto& it : lexer.get_result())
		it.print();
	if (!lexer.is_success())
		std::cout << lexer.get_error();

	std::cout << std::endl;

	tables->print_constants();
	std::cout << '\n';
	tables->print_idetifiers();
	std::cout << '\n';

	Syntaxer syntaxer(&lexer, tables);
	syntaxer.start();
	syntaxer.get_tree().print();
	if (!syntaxer.is_success())
		std::cout << syntaxer.get_error();

	delete tables;
	return 0;
}