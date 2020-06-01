#include "Generator.h"
#include <string>

void Generator::traversal(const std::shared_ptr<struct Tree::node>& node)
{
	if (node->value == "<program>")
	{
		traversal(node->childs.at(1));
		traversal(node->childs.at(3));
	}
	else if (node->value == "<procedure-identifier>")
	{
		program_identifier = node->childs.at(0)->childs.at(0)->value;
		identifiers.push_back(program_identifier);
	}
	else if (node->value == "<block>")
	{
		traversal(node->childs.at(0));
		traversal(node->childs.at(1));
		traversal(node->childs.at(2));
		traversal(node->childs.at(3));
	}
	else if (node->value == "<variable-declaration>")
	{
		if (node->childs.at(0)->value == "<empty>")
			return;
		traversal(node->childs.at(0));
		traversal(node->childs.at(1));
	}
	else if (node->value == "VAR")
	{
		program.append("DATA SEGMENT\n");
		data_exists = true;
	}
	else if (node->value == "<declarations-list>")
	{
		if (node->childs.at(0)->value == "<empty>")
		{
			program.append("DATA ENDS\n");
			return;
		}
		traversal(node->childs.at(0));
		traversal(node->childs.at(1));
	}
	else if (node->value == "<declaration>")
	{
		program.append("\t");
		traversal(node->childs.at(0));
		if (identifier_exists(node->childs.at(0)->childs.at(0)->childs.at(0)->value))
		{
			error << "Generator: Such identifier already exist: " << "line: " + std::to_string(node->childs.at(0)->childs.at(0)->childs.at(0)->line)
				+ " column: " + std::to_string(node->childs.at(0)->childs.at(0)->childs.at(0)->column) + ", identifier: "
				+ node->childs.at(0)->childs.at(0)->childs.at(0)->value + "\n";
		}
		else
		{
			identifiers.push_back(node->childs.at(0)->childs.at(0)->childs.at(0)->value);
		}
		traversal(node->childs.at(2));
		program.append("?\n");
	}
	else if (node->value == "<variable-identifier>")
	{
		program.append(node->childs.at(0)->childs.at(0)->value);
	}
	else if (node->value == "<attribute>")
	{
		if (node->childs.at(0)->value == "INTEGER")
			program.append(" DD ");
		if (node->childs.at(0)->value == "FLOAT")
			program.append(" DD ");
	}
	else if (node->value == "BEGIN")
	{
		program.append("CODE SEGMENT\n\tASSUME CS:CODE");
		if (data_exists) program.append(", DS:DATA\n");
		else program.append("\n");
		program.append(program_identifier + ":\n");
	}
	else if (node->value == "END")
	{
		program.append("MOV AH, 4Ch\nINT 21h\nCODE ENDS\nEND " + program_identifier);
		program.append("\n");
	}
	else if (node->value == "<statement-list>")
	{
		if (node->childs.at(0)->value == "<empty>")
		{
			program.append("\tNOP\n");
			return;
		}
		traversal(node->childs.at(0));
		traversal(node->childs.at(1));
	}
	else if (node->value == "<statement>")
	{
		traversal(node->childs.at(0));
		traversal(node->childs.at(1));
		traversal(node->childs.at(2));
	}
	else if (node->value == "<condition-statement>")
	{
		traversal(node->childs.at(0));
		traversal(node->childs.at(1));
	}
	else if (node->value == "<incomplete-condition-statement>")
	{
		traversal(node->childs.at(1));
		traversal(node->childs.at(3));
	}
	else if (node->value == "<conditional-expression>")
	{
		program.append("\tMOV AX, ");
		traversal(node->childs.at(0));
		program.append("\n");
		program.append("\tMOV BX, ");
		traversal(node->childs.at(2));
		program.append("\n");
		program.append("\tCMP AX, BX\n");
		traversal(node->childs.at(1));
	}
	else if (node->value == "<expression>")
	{
		if (node->childs.at(0)->value == "<variable-identifier>")
		{
			if (!identifier_exists(node->childs.at(0)->childs.at(0)->childs.at(0)->value))
			{
				error << "Generator: No such identifier: " << "line: " + std::to_string(node->childs.at(0)->childs.at(0)->childs.at(0)->line)
					+ " column: " + std::to_string(node->childs.at(0)->childs.at(0)->childs.at(0)->column) + ", identifier: "
					+ node->childs.at(0)->childs.at(0)->childs.at(0)->value + "\n";
			}
			if (node->childs.at(0)->childs.at(0)->childs.at(0)->value == program_identifier)
			{
				error << "Generator: Such identifier already exist: " << "line: " + std::to_string(node->childs.at(0)->childs.at(0)->childs.at(0)->line)
					+ " column: " + std::to_string(node->childs.at(0)->childs.at(0)->childs.at(0)->column) + ", identifier: "
					+ node->childs.at(0)->childs.at(0)->childs.at(0)->value + "\n";
			}
		}
		traversal(node->childs.at(0));
	}
	else if (node->value == "<unsigned-integer>")
	{
		program.append(node->childs.at(0)->value);
	}
	else if (node->value == "=")
	{
		if_stack.push(if_index);
		program.append("\tJNE ?L" + std::to_string(if_index++));
		program.append("\n");
	}
	else if (node->value == "<alternative-part>")
	{
		if (node->childs.at(0)->value == "<empty>")
		{
			int label = if_stack.top();
			if_stack.pop();
			program.append("?L" + std::to_string(label) + ":    NOP\n");
			return;
		}
		traversal(node->childs.at(0));
		traversal(node->childs.at(1));
	}
	else if (node->value == "ELSE")
	{
		else_stack.push(else_index);
		program.append("\tJMP ?LE" + std::to_string(else_index++) + "\n");
		int label = if_stack.top();
		if_stack.pop();
		program.append("?L" + std::to_string(label) + ":    NOP\n");
	}
	else if (node->value == "ENDIF")
	{
		if (else_stack.empty()) return;
		int label = else_stack.top();
		else_stack.pop();
		program.append("?LE" + std::to_string(label) + ":   NOP\n");
		return;
	}
	else if (node->value == ";")
	{
		program.append("\tNOP\n");
	}
}

bool Generator::identifier_exists(std::string identifier)
{
	if (std::find(identifiers.begin(), identifiers.end(), identifier) != identifiers.end())
		return true;
	return false;
}

void Generator::traversal()
{
	this->traversal(tree.root->childs.at(0));
}

