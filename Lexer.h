#pragma once
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

	struct lexem
	{
		int code;
		int row;
		int column;
		std::string name;
		void print() const;
	};
	friend std::ostream& operator<<(std::ostream& stream, const lexem& lex);
	std::vector<struct lexem> lexems;
	InformationTables* tables;
public:
	Lexer(std::ifstream& file, InformationTables* tables);
	~Lexer();
	void start();
	bool is_success() noexcept { return ERROR_NOT_FOUND; }
	std::string get_error() { return error.str(); }
	std::vector<struct lexem> get_result() { return lexems; }
	void write_to_file(std::ostream& stream);
};

