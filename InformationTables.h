#pragma once
#include <unordered_map>

class InformationTables
{
private:
	std::unordered_map<std::string, int> key_words = {
	{"PROGRAM", 301},
	{"BEGIN", 302},
	{"END", 303},
	{"VAR", 304},
	{"FLOAT", 305},
	{"INTEGER", 306},
	{"IF", 307},
	{"ENDIF", 308},
	{"THEN", 309},
	{"ELSE", 310}
	};
	std::unordered_map<std::string, int> identifiers;
	std::unordered_map<std::string, int> constants;
	std::unordered_map<char, int> separators = {
	{'.', (int)'.'},
	{';', (int)';'},
	{':', (int)':'},
	{'=', (int)'='}
	};
	int identifiers_counter = 400;
	int const_counter = 500;

public:
	InformationTables();
	~InformationTables();

	int add_identifier(const std::string& identifier);
	int add_constant(const std::string& constant);
	int get_separator(char c) { return separators[c]; }
	int get_keywords(const std::string& keyword) { return key_words[keyword]; }
	bool is_identifier(int code);
	bool is_constant(int code);
	std::string get_identifier_string(int code);
	std::string get_keyword_string(int code);
	std::string get_constant_string(int code);

	void print_idetifiers();
	void print_constants();
	void write_to_file(std::ostream& stream);
};
