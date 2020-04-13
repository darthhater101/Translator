#pragma once
#include "Lexer.h"
#include "InformationTables.h"
#include "Parser.h"
#include "Generator.h"

class Translator
{
private:
	std::ifstream input;
	struct path 
	{
		std::string project = "./";
		std::string tests_directory = "Tests/";
		std::string tests_kind_directory = "True/";
		std::string test_number = "Test1/";
		std::string input_file = "input.sig";
		std::string output_file = "generated.txt";
		std::string generate_input_path();
		std::string generate_output_path();
	};

	using path_s = struct path;

	path_s path;

public:
	Translator();
	~Translator();
	void translation();
};

