#include "../Phase2/FirstAndFollow/FirstAndFollow.h"
#include "../Phase2/LeftRecursionAndLeftFactoring/HandleLeftFactoring.h"
#include "../Phase2/LeftRecursionAndLeftFactoring/HandleLeftRecursion.h"
#include "../Phase2/ParsingTable/ParsingTable.h"
#include "../Phase2/stack_parser/stack_parser.h"
#include "../Phase1/LexicalAnalyzer.cpp"
#include "LexicalRulesHandler.h"
#include "DFAConverter.h"
#include "MinimizeDFA.h"
#include "../Phase2/ParserRules/ParserRulesLexer.h"
#include <gtest/gtest.h>

using namespace std;
void print_grammar(std::unordered_map<std::string, std::vector<std::vector<std::string>>> grammar){
    for (const auto& [non_terminal, productions] : grammar) {
        std::cout << non_terminal << " -> ";

        for (size_t i = 0; i < productions.size(); ++i) {
            // Print each production
            for (size_t j = 0; j < productions[i].size(); ++j) {
                std::cout << productions[i][j];
                if (j < productions[i].size() - 1) {
                    std::cout << " ";  // Separate symbols in the production
                }
            }
            if (i < productions.size() - 1) {
                std::cout << " | ";  // Separate productions
            }
        }

        std::cout << std::endl;
    }
}

// Test the find_longest_common_prefix method
TEST(Phase2Tests, test1) {
    // Define grammar
    unordered_map<string, vector<vector<string>>> grammar = {
            {"bexpr", {{"bexpr", "or", "bterm"}, {"bterm"}}},
            {"bterm", {{"bterm", "and", "bfactor"}, {"bfactor"}}},
            {"bfactor", {{"not", "bfactor"}, {"(", "bexpr", ")"}, {"true"}, {"false"}}}
    };

    // Define terminals and non-terminals
    set<string> terminals = {"or", "and", "not", "(", ")", "true", "false", EPSILON};
    set<string> non_terminals = {"bexpr", "bterm", "bfactor"};
    vector<string> order_non_terminals = {"bexpr", "bterm", "bfactor"};

    // Apply Left Recursion
    HandleLeftRecursion left_recursion(grammar, non_terminals, order_non_terminals);
    left_recursion.eliminate_left_recursion();
    grammar = left_recursion.getGrammar();
    non_terminals = left_recursion.getNonTerminals();

    // Apply Left Factoring
    HandleLeftFactoring left_factoring(grammar,non_terminals);
    left_factoring.eliminate_left_factoring();
    grammar = left_factoring.getGrammar();
    non_terminals = left_factoring.getNonTerminals();

    // Create First and Follow sets
    FirstAndFollow first_and_follow(grammar, terminals, non_terminals, "bexpr");
    first_and_follow.create_first_and_follow();
    map<string, set<string>> first = first_and_follow.get_first();
    map<string, set<string>> follow = first_and_follow.get_follow();

    // Generate parsing table
    auto parsingTable = ParsingTable::parsing_table_derivation(grammar, terminals, non_terminals, first, follow);

    // Expected parsing table values for validation
    map<string, map<string, vector<string>>> expectedParsingTable = {
            {"bterm`", {
                              {"or", {EPSILON}},
                              {")", {EPSILON}},
                              {"$", {EPSILON}},
                              {"and", {"and", "bfactor", "bterm`"}}
                      }},
            {"bterm", {
                              {"or", {"SYNC"}},
                              {")", {"SYNC"}},
                              {"true", {"bfactor", "bterm`"}},
                              {"$", {"SYNC"}},
                              {"not", {"bfactor", "bterm`"}},
                              {"false", {"bfactor", "bterm`"}},
                              {"(", {"bfactor", "bterm`"}}
                      }},
            {"bexpr`", {
                              {"$", {EPSILON}},
                              {")", {EPSILON}},
                              {"or", {"or", "bterm", "bexpr`"}},
                      }},
            {"bfactor", {
                              {"false", {"false"}},
                              {"true", {"true"}},
                              {"(", {"(", "bexpr", ")"}},
                              {"and", {"SYNC"}},
                              {"or", {"SYNC"}},
                              {")", {"SYNC"}},
                              {"$", {"SYNC"}},
                              {"not", {"not", "bfactor"}}
                      }},
            {"bexpr", {
                              {")", {"SYNC"}},
                              {"true", {"bterm", "bexpr`"}},
                              {"$", {"SYNC"}},
                              {"not", {"bterm", "bexpr`"}},
                              {"false", {"bterm", "bexpr`"}},
                              {"(", {"bterm", "bexpr`"}}
                      }},
    };

    // Add assertions to verify parsing table entries
    for (const auto& non_terminal_entry : expectedParsingTable) {
        const string& non_terminal = non_terminal_entry.first;
        for (const auto& terminal_entry : non_terminal_entry.second) {
            const string& terminal = terminal_entry.first;
            const vector<string>& expected_rule = terminal_entry.second;

            // Check if the expected rule is present in the parsing table
            EXPECT_TRUE(parsingTable[non_terminal].count(terminal) > 0);
            EXPECT_EQ(parsingTable[non_terminal][terminal], expected_rule);
        }
    }

}



TEST(Phase2Tests, MethodBodyGrammar) {
    // Define grammar
    unordered_map<string, vector<vector<string>>> grammar = {
            {"METHOD_BODY", {{"STATEMENT_LIST"}}},
            {"STATEMENT_LIST", {{"STATEMENT"}, {"STATEMENT_LIST", "STATEMENT"}}},
            {"STATEMENT", {{"DECLARATION"},{"IF"},{"WHILE"},{"ASSIGNMENT"}}},
            {"DECLARATION", {{"PRIMITIVE_TYPE", "id", ";"}}},
            {"PRIMITIVE_TYPE", {{"int"}, {"float"}}},
            {"IF", {{"if", "(", "EXPRESSION", ")", "{", "STATEMENT", "}", "else", "{", "STATEMENT", "}"}}},
            {"WHILE", {{"while", "(", "EXPRESSION", ")", "{", "STATEMENT", "}"}}},
            {"ASSIGNMENT", {{"id", "assign", "EXPRESSION", ";"}}},
            {"EXPRESSION", {{"SIMPLE_EXPRESSION"}, {"SIMPLE_EXPRESSION", "relop", "SIMPLE_EXPRESSION"}}},
            {"SIMPLE_EXPRESSION", {{"TERM"},{"SIGN", "TERM"},{"SIMPLE_EXPRESSION", "addop", "TERM"}}},
            {"TERM", {{"FACTOR"}, {"TERM", "mulop", "FACTOR"}}},
            {"FACTOR", {{"id"}, {"num"}, {"(", "EXPRESSION", ")"}}},
            {"SIGN", {{"addop"}}}
    };

    // Define terminals and non-terminals
    set<string> terminals = {"id", "int", "float", ";", "if", "(", ")", "{", "}",
                             "else", "while", "assign", "relop", "addop", "mulop", "num",EPSILON
    };
    set<string> non_terminals = {"METHOD_BODY", "STATEMENT_LIST", "STATEMENT",
                                 "DECLARATION", "PRIMITIVE_TYPE", "IF", "WHILE",
                                 "ASSIGNMENT", "EXPRESSION", "SIMPLE_EXPRESSION",
                                 "TERM", "FACTOR", "SIGN"};
    vector<string> order_non_terminals = {"METHOD_BODY", "STATEMENT_LIST", "STATEMENT",
                                          "DECLARATION", "PRIMITIVE_TYPE", "IF",
                                          "WHILE", "ASSIGNMENT", "EXPRESSION",
                                          "SIMPLE_EXPRESSION", "TERM", "FACTOR",
                                          "SIGN"};

    // Apply Left Recursion
    HandleLeftRecursion left_recursion(grammar, non_terminals, order_non_terminals);
    left_recursion.eliminate_left_recursion();
    grammar = left_recursion.getGrammar();
    non_terminals = left_recursion.getNonTerminals();
//    print_grammar(grammar);

    // Apply Left Factoring
    HandleLeftFactoring left_factoring(grammar,non_terminals);
    left_factoring.eliminate_left_factoring();
    grammar = left_factoring.getGrammar();
    non_terminals = left_factoring.getNonTerminals();
    print_grammar(grammar);

    // Create First and Follow sets
    FirstAndFollow first_and_follow(grammar, terminals, non_terminals, "METHOD_BODY");
    first_and_follow.create_first_and_follow();
    map<string, set<string>> first = first_and_follow.get_first();
    map<string, set<string>> follow = first_and_follow.get_follow();

    // Generate parsing table
    auto parsingTable = ParsingTable::parsing_table_derivation(grammar, terminals, non_terminals, first, follow);

    LexicalRulesHandler handler;
    auto rules = handler.readRules(
            R"(D:\Level_4_Semester_1\Compilers\Project\Compiler-Project\Phase1\rules.txt)");
    rules.pop_back();
    handler.extractKeywords(rules);
    handler.extractPunctuation(rules);
    handler.extractStatements(rules);
//    cout << "---------------------------------------------------" << endl << "DFA:" << endl;
    NFA *nfa = (handler.generateNFAs());
    set<char> alphas = handler.getAlphabet();
    NFAtoDFAConverter nfAtoDfaConverter(*nfa, alphas);
    nfAtoDfaConverter.create_DFA();
    vector<State *> states = nfAtoDfaConverter.get_dfa_states();
    cout << "Number of DFA states: " << states.size() << endl;
    cout << "---------------------------------------------------" << endl << "MinDFA:" << endl;
    MinimizeDFA minDFA;
    set<State *> minStates = minDFA.minDFA(states, nfAtoDfaConverter.get_dfa_transition_table(), alphas);

    map<int, std::unordered_map<char, int>> minTransitionTable = MinimizeDFA::getReducedTransitionTable(minStates);


    FileReader fileReader;
    string path = R"(D:\Level_4_Semester_1\Compilers\Project\Compiler-Project\Phase1\Input\test.txt)";
    vector<string> lines = fileReader.readLines(path);

    LexicalAnalyzer lexicalAnalyzer(lines, minStates);
    FileWriter fileWriter(
            R"(test)");
    vector<string> tokens = lexicalAnalyzer.analyze();
    fileWriter.writeLines(tokens);

    vector<string> new_tokens;
    for(int i=1 ; i<tokens.size() ; i+=2){
        if(tokens[i][0]=='\\')
        {
            new_tokens.push_back(string(tokens[i].begin()+1,tokens[i].end()));
        }
        else{
            new_tokens.push_back(tokens[i]);
        }
    }


    stack_parser stackParser(parsingTable,order_non_terminals[0],new_tokens);
    stackParser.parse(terminals);
    stackParser.display_action();
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}