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
	//bool isPossiblyMatchingDefinedName;

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
	void pushToStack(char op, std::vector<Token>& output);
	// Pops all operations till fiding the opening parenthesis.
	// Throws error if not found.
	void closeParen(std::vector<Token>& output);

	void throwSyntaxError(int i);

	void handleClosure(int i, std::vector<Token>& output);
	void handleUnion(int i, std::vector<Token>& output);
	void handleLeftParen(int i, std::vector<Token>& output);
	void handleRightParen(int i, std::vector<Token>& output);
	void appendRecognizedSequence(std::vector<Token>& output);
	void handleInnerLiteral(int i, std::vector<Token>& output);
	void handleSequenceOpening(int i, std::vector<Token>& output);
	void handleSequenceClosing(int i, std::vector<Token>& output);
	void handleLastLiteral(int i, std::vector<Token>& output);
	void handleLiteral(int i, std::vector<Token>& output);
	void handleEscapeCharacter(int i, std::vector<Token>& output);
	void handleSequenceCharacter(int i, std::vector<Token>& output);

public:
	LexicalRuleParser(
		const std::string& rhs,
		const std::unordered_set<std::string>& definedNames
	);
	std::vector<Token> parse();


};

#endif