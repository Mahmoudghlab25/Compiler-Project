#include "GrammarRulesParser.h"

using namespace std;

void GrammarRulesParser::generateGrammar(vector<string>& tokens) {

    string lhs;
    vector<string> rhs;
    bool isLHS = true;

    for (auto& token: tokens) {
        if (token == "#") {
            if (!lhs.empty() && !rhs.empty()) {
                grammar[lhs].push_back(rhs);
                lhs.clear();
                rhs.clear();
            }
            isLHS = true;
        }
        else if (isLHS) {
            lhs = token;
            grammar[lhs] = {};
            isLHS = false;
            nonTerminals.insert(lhs);
            NonTerminalsOrder.push_back(lhs);
        }
        else if (token == "::=") {
            continue;
        }
        else if (token == "|") {
            grammar[lhs].push_back(rhs);
            rhs.clear();
        }
        else {
            // nonTerminal or terminal in rhs
            rhs.push_back(token);
            if (token[0] == '\'') {
                terminals.insert(string(token.begin()+1,token.end()-1));
            } else {
                nonTerminals.insert(token);
            }
        }
    }
    if (!rhs.empty()) {
        grammar[lhs].push_back(rhs);
        rhs.clear();
        lhs.clear();
    }
}
