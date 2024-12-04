#include "LexicalRuleParser.h"

using namespace std;

void LexicalRuleParser
::populateMatchMap(const unordered_set<string>& definedNames) {
	// generate matchMap from defined names
	for (auto i = definedNames.begin(); i != definedNames.end(); ++i) {
		matchMap[*i] = 0;
	}
	vector<string> specialSequences = {
		"\\L", "\\+", "\\*", "\\-", "\\(", "\\)", "\\\\", "\\|",
		"\\{", "\\}", "\\[", "\\]"
	};
	for (auto& s : specialSequences) {
		matchMap[s] = 0;
	}
}

LexicalRuleParser::LexicalRuleParser(
	const string& rhs,
	const unordered_set<string>& definedNames
) : rhs(rhs), matchMap(), stringBuffer(), opStack() {
	populateMatchMap(definedNames);
}

void LexicalRuleParser::resetStringParsing() {
	stringBuffer = "";
	for (auto& p : matchMap) {
		p.second = 0;
	}
}

void LexicalRuleParser::updateMatches(char c) {
	for (auto& p : matchMap) {
		if (p.second != -1 &&
			p.second < p.first.size() &&
			p.first[(p.second + 1) - 1] == c)
		{
			p.second++;
		}
		else {
			// i >= name.length or c != name[i],
			// ie: stringBuffer is no longer a substring (starting from 
			// first char of the defined name)
			// of the defined name.
			p.second = -1;
		}
	}
}

// Checks if adding the current char c to stringBuffer makes it not match
// any of the defined names or not.
bool LexicalRuleParser::checkMatchReset(char c) {
	for (auto& p : matchMap) {
		if (p.second != -1 &&
			p.second < p.first.size() &&
			p.first[(p.second + 1) - 1] == c)
		{
			// there is a possible match, so keep accepting input
			// (max munch)
			return false;
		}
	}
	return true;
}

bool LexicalRuleParser::completeMatchExists() {
	for (auto& p : matchMap) {
		if (p.second == p.first.size()) {
			// there is a complete match.
			// access the value of stringBuffer to get the matching name
			return true;
		}
	}
	return false;
}

void LexicalRuleParser::pushToStack(char op, vector<Token>& output) {

	if (op == LEFT_PAREN) {
		opStack.push(op);
		return;
	}

	while (!opStack.empty()
		&& opPrecedence[opStack.top()] > opPrecedence[op]) {
		// pop all operations having equal or higher precedence	
		output.push_back(Token(OPERATION, string(1, opStack.top())));
		opStack.pop();
	}
	opStack.push(op);
}

void LexicalRuleParser::closeParen(vector<Token>& output) {
	while (true) {
		if (opStack.empty()) {
			throw runtime_error("Invalid rule: extra closing parenthesis.");
		}
		if (opStack.top() == LEFT_PAREN) {
			opStack.pop();
			break;
		}
		output.push_back(Token(OPERATION, string(1, opStack.top())));
		opStack.pop();
	}
}

void LexicalRuleParser::throwSyntaxError(size_t i) {
	if (i + 1 < rhs.size()) {
		throw runtime_error("Syntax error: "
			+ string(1, rhs[i]) + " then " + rhs[i + 1]);
	}
	else {
		throw runtime_error("Syntax error: "
			+ string(1, rhs[i]) + " at the end of rule RHS.");
	}
}

void LexicalRuleParser::handleClosure(size_t i, vector<Token>& output) {
	if (i + 1 == rhs.size() ||
		rhs[i + 1] == UNION ||
		rhs[i + 1] == RIGHT_PAREN)
	{
		// next = <end>
		pushToStack(rhs[i], output);
	}
	else if (isLiteral(rhs[i + 1]) ||
		rhs[i + 1] == LEFT_PAREN ||
		rhs[i + 1] == ESCAPE)
	{
		pushToStack(rhs[i], output);
		pushToStack(CONCATENATION, output);
	}
	else {
		throwSyntaxError(i);
	}
}

void LexicalRuleParser::handleUnion(size_t i, vector<Token>& output) {
	if (i + 1 < rhs.size() &&
		(isLiteral(rhs[i + 1]) ||
			rhs[i + 1] == LEFT_PAREN ||
			rhs[i + 1] == ESCAPE))
	{
		pushToStack(rhs[i], output);
	}
	else {
		throwSyntaxError(i);
	}
}

void LexicalRuleParser::handleLeftParen(size_t i, vector<Token>& output) {
	if (i + 1 < rhs.size() &&
		(isLiteral(rhs[i + 1]) ||
			rhs[i + 1] == ESCAPE || 
			rhs[i + 1] == LEFT_PAREN))
	{
		pushToStack(rhs[i], output);
	}
	else {
		throwSyntaxError(i);
	}
}

void LexicalRuleParser::handleRightParen(size_t i, vector<Token>& output) {
	if (i + 1 < rhs.size() && rhs[i + 1] == SEQUENCE) {
		throwSyntaxError(i);
	}
	else if (i + 1 < rhs.size() &&
		(isLiteral(rhs[i + 1]) ||
			rhs[i + 1] == ESCAPE) || 
			rhs[i + 1] == LEFT_PAREN)
	{
		closeParen(output);
		pushToStack(CONCATENATION, output);
	}
	else {
		closeParen(output);
	}
}

void LexicalRuleParser::appendRecognizedSequence(vector<Token>& output) {
	if (escapeSeqMap.find(stringBuffer) != escapeSeqMap.end()) {
		// push the escape sequence
		output.push_back(Token(LITERAL, escapeSeqMap[stringBuffer]));
	}
	else {
		// push the defined name
		output.push_back(Token(DEFINED_VAR, stringBuffer));
	}
}

void LexicalRuleParser::handleInnerLiteral(size_t i, vector<Token>& output) {

	// handle sequence closing
	if (!opStack.empty() && opStack.top() == SEQUENCE) {
		handleSequenceClosing(i, output);
		return;
	}

	// if curr char cannot be appended (max munch reached) and
	// there is a complete match
	// Append stringBuffer
	if (checkMatchReset(rhs[i]) && completeMatchExists()) {
		// concat op for the next literal
		pushToStack(CONCATENATION, output);
		appendRecognizedSequence(output);
		// reset stringBuffer and matchMap
		resetStringParsing();
	}
	// if curr char cannot be appended (max munch reached) but
	// no complete matches exist
	// Append stringBuffer
	else if (checkMatchReset(rhs[i]) && !stringBuffer.empty()) {
		// concat op for the next literal
		pushToStack(CONCATENATION, output);
		// add the current value of stringBuffer, if any
		output.push_back(Token(LITERAL, stringBuffer));
		resetStringParsing();
	}
	// append current char
	stringBuffer += rhs[i];
	updateMatches(rhs[i]);
}

void LexicalRuleParser::handleSequenceOpening(size_t i, vector<Token>& output) {

	// guaranteed that next = -

	bool validOpening = (isSequenceOperand(rhs[i]) && stringBuffer.empty()) &&
		((opStack.empty() && output.empty()) ||
			(!opStack.empty() && 
				(opStack.top() == UNION || opStack.top() == LEFT_PAREN)));

	if (!validOpening) {
		throwSyntaxError(i);
	}

	// here: empty stringBuffer guaranteed.
	// just add the current char as a literal
	output.push_back(Token(LITERAL, string(1, rhs[i])));
}

void LexicalRuleParser::handleSequenceClosing(size_t i, vector<Token>& output) {
	bool validClosing = isSequenceOperand(rhs[i]) &&
		(i + 1 == rhs.size() || i + 1 < rhs.size() &&
			(rhs[i + 1] == UNION || rhs[i + 1] == RIGHT_PAREN));

	if (!validClosing) {
		throwSyntaxError(i);
	}

	// here: empty stringBuffer guaranteed.
	// just add the current char as a literal
	output.push_back(Token(LITERAL, string(1, rhs[i])));
}

void LexicalRuleParser::handleLastLiteral(size_t i, vector<Token>& output) {

	// handle sequence opening
	if (i + 1 < rhs.size() &&
		rhs[i + 1] == SEQUENCE)
	{
		handleSequenceOpening(i, output);
		return;
	}

	// check if curr char cannot be added
	// if not then append stringBuffer
	if (checkMatchReset(rhs[i]) && completeMatchExists()) {
		if (i + 1 < rhs.size() && rhs[i + 1] == LEFT_PAREN) {
			pushToStack(CONCATENATION, output);
		}
		appendRecognizedSequence(output);
		resetStringParsing();
		// then process curr char
		output.push_back(Token(LITERAL, string(1, rhs[i])));
	}
	// current char still cannot be added, but no complete matches exist
	else if (checkMatchReset(rhs[i]) && !stringBuffer.empty()) {
		pushToStack(CONCATENATION, output);
		output.push_back(Token(LITERAL, stringBuffer));
		resetStringParsing();
		if (i + 1 < rhs.size() && rhs[i + 1] == LEFT_PAREN) {
			pushToStack(CONCATENATION, output);
		}
		// then process curr char
		output.push_back(Token(LITERAL, string(1, rhs[i])));
	}
	else {
		// else there is still a possibility of getting a match
		// after adding the current char
		stringBuffer += rhs[i];
		updateMatches(rhs[i]);
		if (completeMatchExists()) {
			appendRecognizedSequence(output);
			resetStringParsing();
		}
		else {
			if (i + 1 < rhs.size() && rhs[i + 1] == LEFT_PAREN) {
				pushToStack(CONCATENATION, output);
			}
			output.push_back(Token(LITERAL, stringBuffer));
			resetStringParsing();
		}
	}
}

void LexicalRuleParser::handleLiteral(size_t i, vector<Token>& output) {
	if (i + 1 < rhs.size() && isLiteral(rhs[i + 1])) {
		handleInnerLiteral(i, output);
	}
	else {
		handleLastLiteral(i, output);
	}
}

void LexicalRuleParser::handleEscapeCharacter(size_t i, vector<Token>& output) {
	if (i + 1 < rhs.size() &&
		validEscapeCharacters.find(rhs[i + 1]) != validEscapeCharacters.end())
	{
		handleInnerLiteral(i, output);
	}
	else {
		throwSyntaxError(i);
	}
}

void LexicalRuleParser::handleSequenceCharacter(size_t i, vector<Token>& output) {
	if (i + 1 < rhs.size() && isSequenceOperand(rhs[i + 1])) {
		pushToStack(rhs[i], output);
	}
	else {
		throwSyntaxError(i);
	}
}

vector<Token> LexicalRuleParser::parse() {
	vector<Token> output;

	for (int i = 0; i < rhs.size(); i++) {

		if (isLiteral(rhs[i]) || stringBuffer == "\\") {
			handleLiteral(i, output);
		}
		else if ((rhs[i] == POSITIVE_CLOSURE
			|| rhs[i] == KLEENE_CLOSURE))
		{
			handleClosure(i, output);
		}
		else if (rhs[i] == UNION) {
			handleUnion(i, output);
		}
		else if (rhs[i] == LEFT_PAREN) {
			handleLeftParen(i, output);
		}
		else if (rhs[i] == RIGHT_PAREN) {
			handleRightParen(i, output);
		}
		else if (rhs[i] == ESCAPE) {
			handleEscapeCharacter(i, output);
		}
		else if (rhs[i] == SEQUENCE) {
			handleSequenceCharacter(i, output);
		}
		//ignore whitespace
	}

	// TODO: flush stack

	while (!opStack.empty()) {
		output.push_back(Token(OPERATION, string(1, opStack.top())));
		opStack.pop();
	}

	return output;
}
