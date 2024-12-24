#include "ParserRulesHandler.h"
#include "ParserRulesLexer.h"

using namespace std;

GrammarRulesParser ParserRulesHandler::parseRules(const vector<string>& parserRules) {

    ParserRulesLexer lexer;

    for (auto& rule: parserRules) {
        for (char c: rule) {
            lexer.consume(c);
        }
    }
    lexer.finalizeInput();
    auto tokens = lexer.getTokens();

    GrammarRulesParser grammarParser;
    grammarParser.generateGrammar(tokens);

    return grammarParser;
}