#include "InformationTables.h"
#include <iostream>
#include <iomanip>

InformationTables::InformationTables()
{
}

InformationTables::~InformationTables()
{
}

int InformationTables::add_identifier(const std::string& identifier)
{
	if (key_words.find(identifier) != key_words.end())
		return key_words[identifier];
	if (identifiers.find(identifier) != identifiers.end())
		return identifiers[identifier];
	identifiers.insert(std::make_pair(identifier, ++identifiers_counter));
	return identifiers_counter;
}

int InformationTables::add_constant(const std::string& constant)
{
	if (constants.find(constant) != constants.end())
		return constants[constant];
	constants.insert(std::make_pair(constant, ++const_counter));
	return const_counter;
}

bool InformationTables::is_identifier(int code)
{
	for (const auto& it : identifiers)
	{
		if (it.second == code)
			return true;
	}
	return false;
}

bool InformationTables::is_constant(int code)
{
	for (const auto& it : constants)
	{
		if (it.second == code)
			return true;
	}
	return false;
}

std::string InformationTables::get_identifier_string(int code)
{
	for (const auto& it : identifiers)
	{
		if (it.second == code)
			return it.first;
	}
}

std::string InformationTables::get_keyword_string(int code)
{
	for (const auto& it : key_words)
	{
		if (it.second == code)
			return it.first;
	}
}

std::string InformationTables::get_constant_string(int code)
{
	for (const auto& it : constants)
	{
		if (it.second == code)
			return it.first;
	}
}

void InformationTables::print_idetifiers()
{
	std::cout << "Identifiers table\n";
	std::cout << "-------------------------------------\n";
	for (const auto& it : identifiers)
	{
		std::cout << "| " << std::setw(15) << it.first << " | "
			<< std::setw(15) << it.second << " |\n";
	}
	std::cout << "-------------------------------------\n";
}

void InformationTables::print_constants()
{
	std::cout << "Constants table\n";
	std::cout << "-------------------------------------\n";
	if (constants.empty())
		std::cout << "Empty\n";
	for (const auto& it : constants)
	{
		std::cout << "| " << std::setw(15) << it.first << " | "
			<< std::setw(15) << it.second << " |\n";
	}
	std::cout << "-------------------------------------\n";
}
