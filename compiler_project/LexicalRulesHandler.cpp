#include "LexicalRulesHandler.h"

using namespace std;

string trim(string& s) {

	if (s.empty()) { return string(); }

	int start = 0, end = s.size() - 1;
	while (start < s.size()) {
		if (s[start] != ' ') { break; }
		start++;
	}
	if (start == s.size()) { return string(); }

	while (end >= start) {
		if (s[end] != ' ') { break; }
		end--;
	}

	return string(s, start, end - start + 1);
}

vector<string> splitOnce(const string& s, char splitChar, char splitCharAlt) {
	int i = 0;
	char separator;
	while (i < s.size()) {
		if (s[i] == splitChar || s[i] == splitCharAlt) { break; }
		i++;
		separator = s[i];
	}

	if (i == 0) {
		// op, RHS, no LHS
		return vector<string> {
			string(), separator + "", string(s, i + 1)
		};
	}
	else if (i == s.size() - 1) {
		// LHS, op, no RHS
		return vector<string> {
			string(s, 0, i - 1), separator + "", string()
		};
	}
	else if (i == s.size()) {
		// LHS, no op, no RHS
		return vector<string> {
			string(s, i + 1), string(), string()
		};
	}

	return vector<string> {
		string(s, 0, i - 1), string(s, i + 1)
	};
}

bool checkValidLHS(const string& lhs) {
	if (lhs.empty() || lhs.size() == 1) { return false; }
	for (char c : lhs) {
		if (!isalnum(c) && c != '_') { return false; }
	}
	return true;
}

void LexicalRulesHandler::addStatement(
	unordered_map<string, string>& statementHolder,
	const string& lhs,
	const string& rhs
) {
	if (statementHolder.find(lhs) == statementHolder.end()) {
		statementHolder[lhs] = rhs;
	}
	else {
		statementHolder[lhs] += "|" + rhs;
	}
}

void LexicalRulesHandler::extractStatements(const vector<string>& rules) {


	for (int i = 0; i < rules.size(); i++) {
		// removing any leading/trailing whitespaces
		// TODO: do that after reading from file
		//rule = trim(rule);
		// assuming rules are trimmed
		if (rules[i][0] == LEFT_CURLY || rules[i][0] == LEFT_SQUARE) { continue; }
		vector<string> splittedRule = splitOnce(rules[i], EQUAL, COLON);

		const string& lhs = splittedRule[0];
		const string& op = splittedRule[1];
		const string& rhs = splittedRule[2];

		if (!checkValidLHS(lhs)) {
			throw runtime_error("Invalid LHS at rule " + (i + 1));
		}
		if (op.empty()) {
			// no = or :
			throw runtime_error("Rule " + to_string(i + 1) + " is invalid");
		}
		if (rhs.empty()) {
			// no RHS
			throw runtime_error("Invalid RHS at rule " + (i + 1));
		}

		definedNames.insert(lhs);

		if (op[0] == EQUAL) {
			addStatement(definitions, lhs, rhs);
		}
		else {
			addStatement(expressions, lhs, rhs);
		}
	}
}

vector<Token> LexicalRulesHandler::parseRHS(const string& rhs) {
	int curr = 0, next = 1;
	std::stack<Token> opStack;
	std::string stringBuffer;
	bool isProcessingString = false;

	// skip whitespaces
	return vector<Token>{};
}