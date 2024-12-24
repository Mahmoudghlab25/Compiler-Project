#include "LexicalRulesHandler.h"
#include "LexicalRulesCommon.h"
#include "NFA.h"

using namespace std;

string trim(string &s) {

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

vector<string> splitOnce(const string &s, char splitChar, char splitCharAlt) {
    int i = 0;
    char separator = 0;
    while (i < s.size()) {
        if (s[i] == splitChar || s[i] == splitCharAlt) { break; }
        i++;
        separator = s[i];
    }

    if (i == 0) {
        // op, RHS, no LHS
        return vector<string>{
                string(), string(1, separator), string(s, i + 1)
        };
    } else if (i == s.size() - 1) {
        // LHS, op, no RHS
        return vector<string>{
                string(s, 0, i - 1), string(1, separator), string()
        };
    } else if (i == s.size()) {
        // LHS, no op, no RHS
        return vector<string>{
                string(s, i + 1), string(), string()
        };
    }

    return vector<string>{
            string(s, 0, i), string(1, separator), string(s, i + 1)
    };
}

vector<string> LexicalRulesHandler::readRules(const char *fileName) {
    vector<string> lines;
    ifstream file(fileName);

    while (true) {
        string line;
        if (!getline(file, line)) {
            line = trim(line);
            lines.push_back(line);
            break;
        }
        lines.push_back(line);
    }

    return lines;
}

bool checkValidLHS(const string &lhs) {
    if (lhs.empty() || lhs.size() == 1) { return false; }
    for (char c: lhs) {
        if (!isalnum(c) && c != '_') { return false; }
    }
    return true;
}

void LexicalRulesHandler::addStatement(
        unordered_map<string, string> &statementHolder,
        const string &lhs,
        const string &rhs
) {
    if (statementHolder.find(lhs) == statementHolder.end()) {
        statementHolder[lhs] = rhs;
    } else {
        statementHolder[lhs] += "|" + rhs;
    }
}

void LexicalRulesHandler::extractStatements(const vector<string> &rules) {
    for (int i = 0; i < rules.size(); i++) {
        // assuming rules are trimmed
        if (rules[i][0] == LEFT_CURLY || rules[i][0] == LEFT_SQUARE) { continue; }
        vector<string> splittedRule = splitOnce(rules[i], EQUAL, COLON);
        const string &lhs = trim(splittedRule[0]);
        const string &op = trim(splittedRule[1]);
        const string &rhs = trim(splittedRule[2]);
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
        allNames.insert(lhs);
        addStatement(statements, lhs, rhs);
        if (op[0] == COLON) {
            expNames.insert(lhs);
            order.push_back(lhs);
        }
    }
}

void LexicalRulesHandler::extractKeywords(const vector<string> &rules) {
    for (int i = 0; i < rules.size(); i++) {
        // removing any leading/trailing whitespaces
        // TODO: do that after reading from file
        //rule = trim(rule);
        // assuming rules are trimmed

        if (rules[i][0] != LEFT_CURLY) { continue; }
        string rule(rules[i], 1, rules[i].size() - 2);
        istringstream stream(rule);
        string keyword;
        while (stream >> keyword) {
            keyWords.insert(keyword);
            order.push_back(keyword);
        }
    }
}

void LexicalRulesHandler::extractPunctuation(const vector<string> &rules) {
    for (int i = 0; i < rules.size(); i++) {
        // removing any leading/trailing whitespaces
        // TODO: do that after reading from file
        //rule = trim(rule);
        // assuming rules are trimmed

        if (rules[i][0] != LEFT_SQUARE) { continue; }
        string rule(rules[i], 1, rules[i].size() - 2);
        istringstream stream(rule);
        string punc;

        while (stream >> punc) {
            punctuation.insert(punc);
            if (escapeSeqMap.find(punc) == escapeSeqMap.end()) {
                order.push_back(punc);
            } else {
                order.push_back(escapeSeqMap[punc]);
            }
        }
    }
}

vector<Token> LexicalRulesHandler::parseRHS(const string &rhs) {
    int curr = 0, next = 1;
    std::stack<Token> opStack;
    std::string stringBuffer;
    bool isProcessingString = false;

    // skip whitespaces
    return vector<Token>{};
}

NFA *LexicalRulesHandler::generateNFAForPunctuation(const string &punc) {
    auto parser = LexicalRuleParser(punc, allNames);
    auto puncToken = parser.parse()[0];
    addToAlphabet(puncToken.value);
    NFA *n = new NFA();
    return n->basic(puncToken.value[0]);
}

void LexicalRulesHandler::addToAlphabet(char c) {
    alphabet.insert(c);
}

void LexicalRulesHandler::addToAlphabet(const string &val) {
    for (const char c: val) {
        alphabet.insert(c);
    }
}

NFA *LexicalRulesHandler::generateNFA(const string &curr) {
    if (nfaMap.find(curr) != nfaMap.end()) {
        // if found, copy then return
        return new NFA(*nfaMap[curr]);
    }
    // else calculate, store in map, copy and return
    auto parser = LexicalRuleParser(statements.at(curr), allNames);
    vector<Token> tokens = parser.parse();
    stack<NFA *> stack;
    for (Token &token: tokens) {
        if (token.type == OPERATION) {
            switch (token.value[0]) {
                NFA *op1, *op2, *op, *res, *temp;

                case UNION:
                    if (stack.empty()) {
                        throw runtime_error("Union operation missing 1st operand");
                    }
                    op2 = stack.top();
                    stack.pop();
                    if (stack.empty()) {
                        throw runtime_error("Union operation missing 2nd operand");
                    }
                    op1 = stack.top();
                    stack.pop();
                    res = new NFA();
                    stack.push(res->Union(*op1, *op2));
                    break;
                case CONCATENATION:
                    if (stack.empty()) {
                        throw runtime_error("Concat operation missing 1st operand");
                    }
                    op2 = stack.top();
                    stack.pop();
                    if (stack.empty()) {
                        throw runtime_error("Concat operation missing 2nd operand");
                    }
                    op1 = stack.top();
                    stack.pop();
                    res = new NFA();
                    stack.push(res->Concatenate(*op1, *op2));
                    break;
                case KLEENE_CLOSURE:
                    if (stack.empty()) {
                        throw runtime_error("Kleene closure operation missing 1st operand");
                    }
                    op = stack.top();
                    stack.pop();
                    res = new NFA();
                    stack.push(res->Closure(*op));
                    break;
                case POSITIVE_CLOSURE:
                    if (stack.empty()) {
                        throw runtime_error("Positive closure operation missing 1st operand");
                    }
                    op = stack.top();
                    stack.pop();
                    res = new NFA();
                    stack.push(res->positive_closure(*op));
                    break;
                case SEQUENCE:
                    if (stack.empty()) {
                        throw runtime_error("Concat operation missing 1st operand");
                    }

                    op2 = stack.top();
                    stack.pop();
                    char symbol2 = op2->getTransitions()[0].symbol;
                    if (stack.empty()) {
                        throw runtime_error("Concat operation missing 2nd operand");
                    }

                    op1 = stack.top();
                    stack.pop();
                    char symbol1 = op1->getTransitions()[0].symbol;

                    //a-9 not handled
                    auto start = seqString.find(symbol1);
                    auto end = seqString.find(symbol2);
                    string seq = seqString.substr(
                            start,
                            end - start + 1
                    );
                    addToAlphabet(seq);
                    //"a..z"
                    res = new NFA();
                    stack.push(res->nfa_options(seq, curr));
                    break;
            }
        } else if (token.type == LITERAL) {
            if (token.value.size() == 1) {
                addToAlphabet(token.value[0]);
                NFA *n = new NFA();
                stack.push(n->basic(token.value[0]));
            } else {
                addToAlphabet(token.value);
                NFA *n = new NFA();
                stack.push(n->nfa_sequence(token.value, curr));
            }
        } else {
            NFA *definedVarNFA = generateNFA(token.value);
            stack.push(definedVarNFA);
        }
    }
    // store nfa of curr in map
    NFA *res = stack.top();
    stack.pop();
    if (!stack.empty()) {
        throw runtime_error("error while parsing rhs [stack not empty]");
    }
    /*nfaMap[curr] = res;*/
    return res;
}

NFA *LexicalRulesHandler::generateNFAs() {

    for (auto &p: statements) {
        if (nfaMap.find(p.first) == nfaMap.end()) {
            nfaMap[p.first] = generateNFA(p.first);
        }
    }

    // remove definitions, not needed anymore
    vector<string> unneeded;
    for (auto &[var, _]: nfaMap) {
        if (expNames.find(var) == expNames.end()) {
            unneeded.push_back(var);
        }
    }
    for (const string &var: unneeded) {
        nfaMap.erase(var);
    }

    // loop on keywords, punc --> nfa
    for (auto &keyword: keyWords) {
        addToAlphabet(keyword);
        NFA *nfa = new NFA();
        nfaMap[keyword] = nfa->nfa_sequence(keyword, keyword);
    }

    for (auto &punc: punctuation) {
        if (escapeSeqMap.find(punc) == escapeSeqMap.end()) {
            nfaMap[punc] = generateNFAForPunctuation(punc);
        } else {
            nfaMap[escapeSeqMap[punc]] = generateNFAForPunctuation(punc);
        }
    }

    NFA *res = new NFA();
    return res->combine(nfaMap, order);
}

set<char> LexicalRulesHandler::getAlphabet() {
    return alphabet;
}