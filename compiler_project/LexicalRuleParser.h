#ifndef LEXICALRULEPARSER_H
#define LEXICALRULEPARSER_H
#include "LexicalRulesCommon.h"

class LexicalRuleParser {
private:
	// rhs value to be parsed
	const std::string& rhs;
	std::unordered_map<std::string, int> matchMap;
	std::string stringBuffer;
	std::stack<char> opStack;
	std::vector<Token> output;

	void populateMatchMap(const std::unordered_set<std::string>& definedNames);
	// resets matchMap and stringBuffer
	void resetStringParsing();

	// Updates matchMap based on the current char and its position
	// matchMap[x] = -1 if stringBuffer does not represent a substring of x
	// that starts from the start of x
	void updateMatches(char c);
	// Checks if adding the current char c to stringBuffer makes it not match
	// any of the defined names or not.
	bool checkMatchReset(char c);
	// Checks if stringBuffer completely matches one of the defined names or not.
	bool completeMatchExists();

	// Pushes operation to stack after poping all higher or equal precedence
	// operations.
	void pushToStack(char op);
	// Pops all operations till fiding the opening parenthesis.
	// Throws error if not found.
	void closeParen();

	void throwSyntaxError(size_t i);

	void handleClosure(size_t i);
	void handleUnion(size_t i);
	void handleLeftParen(size_t i);
	void handleRightParen(size_t i);
	void appendRecognizedSequence();
	void handleInnerLiteral(size_t i);
	void handleSequenceOpening(size_t i);
	void handleSequenceClosing(size_t i);
	void handleLastLiteral(size_t i);
	void handleLiteral(size_t i);
	void handleEscapeCharacter(size_t i);
	void handleSequenceCharacter(size_t i);

public:
	LexicalRuleParser(
		const std::string& rhs,
		const std::unordered_set<std::string>& definedNames
	);
	std::vector<Token> parse();


};

#endif