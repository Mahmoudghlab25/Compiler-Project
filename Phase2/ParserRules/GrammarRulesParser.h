#ifndef GRAMMARRULESPARSER_H
#define GRAMMARRULESPARSER_H

#include "common.h"

typedef std::unordered_map<std::string, std::vector<std::vector<std::string>>>
Grammar;

struct GrammarRulesParser {
    std::set<std::string> terminals;
    std::set<std::string> nonTerminals;
    std::vector<std::string> NonTerminalsOrder;
    Grammar grammar;

    void generateGrammar(std::vector<std::string>& tokens);
};



#endif //GRAMMARRULESPARSER_H
