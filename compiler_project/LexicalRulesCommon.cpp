#include "LexicalRulesCommon.h"

std::unordered_map<char, int> opPrecedence{
	{UNION, 0},
	{CONCATENATION, 1},
	{KLEENE_CLOSURE, 2},
	{POSITIVE_CLOSURE, 2},
	{SEQUENCE, 3},
};

// "a..zA..Z0..9"
// string(0, 25) a..z
// string(26, 51) A..Z
// string(52, 61) 0..9

std::string seqString = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

std::unordered_set<char> specialSymbols = {
	'-', '+', '*', '|', '(', ')', '\\'
};

std::unordered_map<std::string, std::string> escapeSeqMap = {
	{"\\L", "\0"},
	{"\\+", "+"},
	{"\\*", "*"},
	{"\\-", "-"},
	{"\\(", "("},
	{"\\)", ")"},
	{"\\\\", "\\"},
	{"\\|", "|"},
	{"\\{", "{"},
	{"\\}", "}"},
	{"\\[", "["},
	{"\\]", "]"}
};

std::unordered_set<char> validEscapeCharacters = {
	'L', '-', '+', '*', '|', '(', ')', '\\', '[', ']', '{', '}'
};

Token::Token(TokenType type, const std::string& value) :
	type(type), value(value) {}

bool isLiteral(char c) {
	return specialSymbols.find(c) == specialSymbols.end()
		&& !std::isspace(static_cast<unsigned char>(c));
}

bool isSequenceOperand(char c) {
	return ('0' <= c && c <= '9') ||
		('a' <= c && c <= 'z') ||
		('A' <= c && c <= 'Z');
}

bool Token::operator==(const Token& other) const {
	return type == other.type && value == other.value;
}