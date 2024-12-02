#ifndef LEXICALRULESCOMMON_H
#define LEXICALRULESCOMMON_H

#include "common.h"

typedef enum Symbol {
	SEQUENCE = '-',
	POSITIVE_CLOSURE = '+',
	KLEENE_CLOSURE = '*',
	CONCATENATION = '.',
	UNION = '|',
	LEFT_PAREN = '(',
	RIGHT_PAREN = ')',
	ESCAPE = '\\',
	LEFT_SQUARE = '[',
	RIGHT_SQUARE = ']',
	LEFT_CURLY = '{',
	RIGHT_CURLY = '}',
	EQUAL = '=',
	COLON = ':'
} Symbol;


typedef enum TokenType {
	OPERATION,
	LITERAL,
	DEFINED_VAR,
	PAREN,
	END
} TokenType;

typedef struct Token {
	TokenType type;
	const std::string value;
	Token(TokenType type, const std::string& value);
} Token;

extern std::unordered_map<char, int> opPrecedence;

extern std::unordered_set<char> specialSymbols;

// value is string bec Token value is of type string
extern std::unordered_map<std::string, std::string> escapeSeqMap;

extern std::unordered_set<char> validEscapeCharacters;

bool isLiteral(char c);
bool isSequenceOperand(char c);

#endif