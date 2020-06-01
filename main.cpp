#include "Lexer.h"
#include "InformationTables.h"
#include "Parser.h"
#include "Translator.h"
#include <iostream>
#include <cstdlib>

int main(int argc, char **args)
{
	std::string path = args[1];
	std::cout << path << std::endl;
	Translator translator(path);
	translator.translation();
	return 0;
}