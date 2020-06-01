#include "Translator.h"
#include <cstdlib>

Translator::Translator(std::string path) : path(path)
{
	input.open(path + "input.sig");
	if (!input.is_open())
	{
		std::cout << "No such file" << std::endl;
		exit(-1);
	}
}

Translator::~Translator()
{
}

void Translator::translation()
{
	InformationTables* tables = new InformationTables();
	Lexer lexer(input, tables);
	lexer.start();
	std::ofstream output(path + "generated.txt");

	auto res = lexer.get_result();
	if (!lexer.is_success())
	{
		std::cout << lexer.get_error();
		output << lexer.get_error();
		system("pause");
		exit(-1);
	}

	std::shared_ptr<InformationTables> tables_ptr = std::make_shared<InformationTables>(*tables);
	Parser parser(res, tables_ptr);
	parser.start();
	if (!parser.is_success())
	{
		std::cout << parser.get_error();
		output << parser.get_error();
		system("pause");
		exit(-1);
	}

	Generator generator(parser.get_tree(), tables_ptr);
	generator.traversal();
	generator.print();
	std::cout << generator.get_error();
	generator.write_to_file(output);
	delete tables;
}
