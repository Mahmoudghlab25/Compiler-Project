#include "../ParserRules/ParserRulesLexer.h"
#include "../../Phase1/common.h"
#include <gtest/gtest.h>

using namespace std;

TEST(Phase2Tests, BasicParserRuleLexing) {
    ParserRulesLexer lexer;
    vector<string> rules = { "# SIGN ::= 'addop'" };
    for (auto& rule: rules) {
        for (char c: rule) {
            lexer.consume(c);
        }
    }
    lexer.finalizeInput();
    auto tokens = lexer.getTokens();
    vector<string> expectedTokens = {"#", "SIGN", "::=", "'addop'"};
    EXPECT_EQ(tokens, expectedTokens);
}

TEST(Phase2Tests, TwoNonTerminals) {
    ParserRulesLexer lexer;
    vector<string> rules = { "# STATEMENT_LIST ::= STATEMENT_LIST STATEMENT" };
    for (auto& rule: rules) {
        for (char c: rule) {
            lexer.consume(c);
        }
    }
    lexer.finalizeInput();
    auto tokens = lexer.getTokens();
    vector<string> expectedTokens = {"#", "STATEMENT_LIST", "::=", "STATEMENT_LIST", "STATEMENT"};
    EXPECT_EQ(tokens, expectedTokens);
}