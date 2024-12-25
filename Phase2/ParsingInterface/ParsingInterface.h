#ifndef COMPILER_PROJECT_PARSINGINTERFACE_H
#define COMPILER_PROJECT_PARSINGINTERFACE_H
#include<bits/stdc++.h>
#include "../Phase1/LexicalAnalyzer.cpp"
#include "../Phase1/LexicalRulesHandler.h"
#include "../Phase1/NFA.h"
#include "../Phase1/DFAConverter.h"
#include "../Phase1/MinimizeDFA/MinimizeDFA.h"
#include "../Phase1/FileReader.cpp"
#include "../Phase2/Symbols.h"
#include "../Phase2/LeftRecursionAndLeftFactoring/HandleLeftFactoring.h"
#include "../Phase2/LeftRecursionAndLeftFactoring/HandleLeftRecursion.h"
#include "../FirstAndFollow/FirstAndFollow.h"
#include "../ParsingTable/ParsingTable.h"
#include "../ParserRules/ParserRulesHandler.h"
class ParsingInterface{
private:
    const char * parser_input;
    const char * rules_input;
    const char * program_input;
    FileWriter file_writer;
    std::string  output_path;
    vector<string> tokens_to_parser;
    Grammar grammar;
    set<string> terminals;
    set<string> non_terminals;
    vector<string> order_non_terminals;
    map<string, set<string>> first;
    map<string, set<string>> follow;
    unordered_map<string, unordered_map<string, vector<string>>> predictive_table;
    queue<string> actions;
    set<State *> min_states;

public:
    ParsingInterface(const char *parserInput, const char *rulesInput, const char *programInput, std::string output_path);

    void compute_min_states();

    void compute_grammar();

    void compute_left_recursion_and_factoring();

    void compute_first_and_follow_and_predictive_table();

    void compute_actions();

    void compute_parser();

};

#endif //COMPILER_PROJECT_PARSINGINTERFACE_H
