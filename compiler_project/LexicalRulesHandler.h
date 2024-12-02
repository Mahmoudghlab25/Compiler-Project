#pragma once
#pragma once
#include "LexicalRulesCommon.h"
#include "LexicalRuleParser.h"


class LexicalRulesHandler {
private:
	std::unordered_set<std::string> definedNames;
	// definitions
	std::unordered_map<std::string, std::string> definitions;
	// expressions
	std::unordered_map<std::string, std::string> expressions;
	//std::unordered_map<int, vector<int>> ruleDependencyGraph;

	// Adds the statement to its corresponding map
	// If lhs already exists, new rhs is joined with the existing rhs by '|'
	void addStatement(
		std::unordered_map<std::string, std::string>& statementHolder,
		const std::string& lhs,
		const std::string& rhs
	);

	std::vector<Token> parseRHS(const std::string& rhs);


public:
	/*
		Accepts rules read from input file (which are expected to be trimmed).
		Extracts statements (expressions/definitions)
		for each defined variable, then pushes
		the rhs to the corresponding map.
		If lhs already exists, the existing rhs is joined with the new rhs
		with a union '|' operation.
		Defined names are stored in a set.
	*/
	void extractStatements(const std::vector<std::string>& rules);
};