#include "Generator.h"
#include <string>

void Generator::traversal(std::shared_ptr<struct Tree::node>& node)
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
	else if (node->value == "<variable-declarations>")
	{
		if (node->childs.at(0)->value == "<empty>")
			return;
		traversal(node->childs.at(0));
		traversal(node->childs.at(1));
	}
	else if (node->value == "VAR")
	{
		program.append("DATA SEGMENT\n");
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
		traversal(node->childs.at(0));
		if (identifier_exists(node->childs.at(0)->childs.at(0)->childs.at(0)->value))
		{
			error << "Such identifier already exist\n";
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
			program.append(" DW ");
		if (node->childs.at(0)->value == "FLOAT")
			program.append(" DW ");
	}
	else if (node->value == "BEGIN")
	{
		program.append("CODE SEGMENT\n\tASSUME CS:CODE, DS:DATA\n");
		program.append(program_identifier + ":\n");
	}
	else if (node->value == "END")
	{
		program.append("MOV AH, 4Ch\nINT 21h\nCODE ENDS\nEND " + program_identifier);
	}
	else if (node->value == "<statement-list>")
	{
		if (node->childs.at(0)->value == "<empty>")
		{
			return;
		}
		traversal(node->childs.at(0));
		traversal(node->childs.at(1));
	}
	else if (node->value == "<statement>")
	{
		traversal(node->childs.at(0));
		traversal(node->childs.at(1));
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
		program.append("MOV AX, ");
		traversal(node->childs.at(0));
		program.append("\n");
		program.append("MOV BX, ");
		traversal(node->childs.at(2));
		program.append("\n");
		program.append("CMP AX, BX\n");
		traversal(node->childs.at(1));
	}
	else if (node->value == "<expression>")
	{
		if (!identifier_exists(node->childs.at(0)->childs.at(0)->childs.at(0)->value))
		{
			error << "No such identifier\n";
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
		program.append("JNE ?L" + std::to_string(if_index++));
		program.append("\n");
	}
	else if (node->value == "<alternative-part>")
	{
		if (node->childs.at(0)->value == "<empty>")
		{
			int label = if_stack.top();
			if_stack.pop();
			program.append("?L" + std::to_string(label) + ":NOP\n");
			return;
		}
		traversal(node->childs.at(0));
		traversal(node->childs.at(1));
	}
	else if (node->value == "ELSE")
	{
		else_stack.push(else_index);
		program.append("JMP ?LE" + std::to_string(else_index++) + "\n");
		int label = if_stack.top();
		if_stack.pop();
		program.append("?L" + std::to_string(label) + ":NOP\n");
	}
	else if (node->value == "ENDIF")
	{
		if (else_stack.empty()) return;
		int label = else_stack.top();
		else_stack.pop();
		program.append("?LE" + std::to_string(label) + ":NOP\n");
		return;
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

