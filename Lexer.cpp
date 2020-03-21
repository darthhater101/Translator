#include "Lexer.h"
#include <iostream>
#include <iomanip>

Lexer::Lexer(std::ifstream& file, InformationTables* tables)
{
	for (int i = 0; i < 128; i++)
		attributes[i] = 5;
	attributes[8] = 0;
	attributes[9] = 0;
	attributes[10] = 0;
	attributes[13] = 0;
	attributes[32] = 0;
	for (int i = 48; i < 58; i++)
		attributes[i] = 1;
	for (int i = 58; i < 63; i++)
		attributes[i] = 3;
	for (int i = 65; i < 91; i++)
		attributes[i] = 2;
	for (int i = 97; i < 123; i++)
		attributes[i] = 2;
	attributes[46] = 3;
	attributes[44] = 3;
	attributes[40] = 4;
	this->tables = tables;
	error << "Lexer error: ";
	this->file.swap(file);
}

Lexer::~Lexer()
{
	file.close();
}

void Lexer::start()
{
	char c;
	int row = 1;
	int column = 1;
	int lexem_column;
	std::string buff = "";

	c = file.get();
	while (!file.eof())
	{
		if (attributes[c] == 0)
		{
			if (c == 10 || c == 13)
			{
				row++;
				column = 1;
			}
			else if (c == 32)
				column++;
			else if (c == 9)
				column += 4;
			c = file.get();
		}

		if (attributes[c] == 1)
		{
			lexem_column = column;
			while (attributes[c] == 1)
			{
				buff += c;
				c = file.get();
				column++;
			}
			if (attributes[c] == 2)
			{
				ERROR_NOT_FOUND = false;
				error << "Error in line: " << row << ", position: " << lexem_column
					<< ", unexpected character '" << c << "'\n";
				break;
			}
			lexems.push_back({ tables->add_constant(buff), row, lexem_column, buff });
			buff.clear();
		}
		else if (attributes[c] == 2)
		{
			lexem_column = column;
			c = toupper(c);
			while (attributes[c] == 2 || attributes[c] == 1)
			{
				buff += c;
				c = toupper(file.get());
				column++;
			}
			lexems.push_back({ tables->add_identifier(buff), row, lexem_column, buff });
			buff.clear();
		}
		else if (attributes[c] == 3)
		{
			lexem_column = column;
			buff += c;
			lexems.push_back({ tables->get_separator(c), row, lexem_column, buff });
			buff.clear();
			c = file.get();
			column++;
		}
		else if (attributes[c] == 4)
		{
			if (file.eof())
			{
				ERROR_NOT_FOUND = false;
				error << "Error in line: " << row << ", position: " << column
					<< ", illegal character '" << c << "'\n";
			}
			else
			{
				c = file.get();
				column++;
				if (c == 42)
				{
					if (file.eof())
					{
						ERROR_NOT_FOUND = false;
						error << "'*)' expected but eof found\n";
						break;
					}
					else
					{
						c = file.get();
						column++;
						do
						{
							while (!file.eof() && c != 42)
							{
								if (c == 10 || c == 13)
								{
									row++;
									column = 1;
								}
								c = file.get();
								column++;
							}
							if (file.eof())
							{
								ERROR_NOT_FOUND = false;
								error << "'*)' expected but eof found\n";
								break;
							}
							else
							{
								c = file.get();
								column++;
							}
						} while (c != 41);
						c = file.get();
						column++;
					}
				}
			}
		}
		else if (attributes[c] == 5)
		{
			ERROR_NOT_FOUND = false;
			error << "Error in line: " << row << ", position: " << column
				<< ", illegal character '" << c << "'\n";
			break;
		}
	}
	file.close();
}

void Lexer::write_to_file(std::ostream& stream)
{
	for (const auto& it : lexems)
		stream << it;
	stream << std::endl;
}

void Lexer::lexem::print() const
{
	std::cout << std::setw(2) << row << std::setw(4) << column
		<< std::setw(5) << code << std::setw(10) << name << "\n";
}

std::ostream& operator<<(std::ostream& stream, const struct Lexer::lexem& lex)
{
	stream << std::setw(2) << lex.row << std::setw(4) << lex.column
		<< std::setw(5) << lex.code << std::setw(10) << lex.name << "\n";
	return stream;
}
