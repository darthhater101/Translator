#include "Translator.h"
#include <cstdlib>

Translator::Translator()
{
	input.open(path.generate_input_path());
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
	std::ofstream output(path.generate_output_path());

	auto res = lexer.get_result();
	lexer.write_to_file(output);
	for (const auto& it : res)
		it.print();
	if (!lexer.is_success())
	{
		std::cout << lexer.get_error();
		output << lexer.get_error();
		system("pause");
		exit(-1);
	}

	std::cout << std::endl;
	tables->print_constants();
	tables->print_idetifiers();
	tables->write_to_file(output);

	std::shared_ptr<InformationTables> tables_ptr = std::make_shared<InformationTables>(*tables);
	Parser parser(res, tables_ptr);
	parser.start();
	parser.get_tree().print();
	parser.get_tree().write_to_file(output);
	if (!parser.is_success())
	{
		std::cout << parser.get_error();
		output << parser.get_error();
		system("pause");
		exit(-1);
	}
	std::cout << "\n\n\n";
	Generator generator(parser.get_tree(), tables_ptr);
	generator.traversal();
	generator.print();
	std::cout << generator.get_error();
	generator.write_to_file(output);
	delete tables;
}

std::string Translator::path::generate_input_path()
{
	return project + tests_directory + tests_kind_directory + test_number + input_file;
}

std::string Translator::path::generate_output_path()
{
	return project + tests_directory + tests_kind_directory + test_number + output_file;
}
