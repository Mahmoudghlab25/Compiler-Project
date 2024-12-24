#ifndef PARSERRULESHANDLER_H
#define PARSERRULESHANDLER_H

#include "common.h"
#include "DFAConverter.h"
#include "MinimizeDFA.h"
#include "GrammarRulesParser.h"

typedef std::unordered_map<std::string, std::vector<std::vector<std::string>>>
Grammar;

class ParserRulesHandler {
public:
    GrammarRulesParser parseRules(const std::vector<std::string>& parserRules);
};



#endif //PARSERRULESHANDLER_H
