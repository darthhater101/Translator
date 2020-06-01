#pragma once
#include "Lexer.h"
#include "InformationTables.h"
#include "Parser.h"
#include "Generator.h"

class Translator
{
private:
	std::ifstream input;
	std::string path;
public:
	Translator(std::string path);
	~Translator();
	void translation();
};

