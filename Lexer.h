#pragma once
#pragma warning(disable: 4996)
#include "InformationTables.h"
#include <fstream>
#include <sstream>

class Lexer
{
private:
	std::ifstream file;
	char attributes[128];
	bool ERROR_NOT_FOUND = true;
	std::stringstream error;

	typedef struct lexem
	{
		int code;
		int row;
		int column;
		std::string name;
		void print() const;
	} lexem_t;
	std::vector<lexem_t> lexems;
	InformationTables* tables;
public:
	Lexer(const std::string& path, InformationTables* tables);
	~Lexer();
	void start();
	bool is_success() { return ERROR_NOT_FOUND; }
	std::string get_error() { return error.str(); }
	std::vector<lexem_t> get_result() { return lexems; }
};

