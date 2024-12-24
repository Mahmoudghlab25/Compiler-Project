//
// Created by Mahmo on 12/22/2024.
//
#include <iostream>
#include <set>
#include <map>
#include <vector>
#include "ParsingTable.h"
#include "../FirstAndFollow/FirstAndFollow.h"
#include "../LeftRecursionAndLeftFactoring/HandleLeftFactoring.h"
#include "../LeftRecursionAndLeftFactoring/HandleLeftRecursion.h"

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
void test1() {
    // Define grammar
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> grammar = {
            {"S", {{"R", "T"}}},
            {"R", {{"s", "U", "R", "b"}, {EPSILON}}},
            {"U", {{"u", "U"},           {EPSILON}}},
            {"V", {{"v", "V"},           {EPSILON}}},
            {"T", {{"V", "t", "T"},      {EPSILON}}}
    };

    // Define terminals and non-terminals
    std::set<std::string> terminals = {"s", "u", "v", "t", "b", EPSILON};
    std::set<std::string> non_terminals = {"S", "R", "U", "V", "T"};

    // Define first sets
    std::map<std::string, std::set<std::string>> first = {
            {"S", {"s", "v", "t", EPSILON}},
            {"R", {"s", EPSILON}},
            {"U", {"u", EPSILON}},
            {"V", {"v", EPSILON}},
            {"T", {"v", "t", EPSILON}}
    };

    // Define follow sets
    std::map<std::string, std::set<std::string>> follow = {
            {"S", {"$"}},
            {"R", {"v", "t", "b", "$"}},
            {"U", {"s", "b"}},
            {"V", {"t"}},
            {"T", {"$"}}
    };

    // Generate parsing table
    auto parsingTable = ParsingTable::parsing_table_derivation(grammar, terminals, non_terminals, first, follow);

    // Print parsing table for verification
    for (const auto &non_terminal_entry: parsingTable) {
        std::cout << "Non-terminal: " << non_terminal_entry.first << "\n";
        for (const auto &terminal_entry: non_terminal_entry.second) {
            std::cout << "  Terminal: " << terminal_entry.first << " -> [ ";
            for (const auto &symbol: terminal_entry.second) {
                std::cout << symbol << " ";
            }
            std::cout << "]\n";
        }
    }
}

void test2() {
    // Define grammar
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> grammar = {
            {"E",     {{"T", "E`"}}},
            {"E`", {{"+", "T", "E`"}, {EPSILON}}},
            {"T",     {{"F", "T`"}}},
            {"T`", {{"*", "F", "T`"}, {EPSILON}}},
            {"F",     {{"(", "E", ")"},     {"id"}}}
    };

    // Define terminals and non-terminals
    std::set<std::string> terminals = {"+", "*", "(", ")", "id", EPSILON};
    std::set<std::string> non_terminals = {"E", "E`", "T", "T`", "F"};
    std::vector<std::string> order_non_terminals = {"E", "E`", "T", "T`", "F"};

//    Apply Left Recursion and Left Factoring
    HandleLeftRecursion left_recursion(grammar,non_terminals,order_non_terminals);
    left_recursion.eliminate_left_recursion();
    grammar = left_recursion.getGrammar();

    HandleLeftFactoring left_factoring(grammar);
    left_factoring.eliminate_left_factoring();
    grammar = left_factoring.getGrammar();

//    Create first and follow
    FirstAndFollow first_and_follow(grammar,terminals,non_terminals,"E");
    first_and_follow.create_first_and_follow();
    std::map<std::string, std::set<std::string>> first = first_and_follow.get_first();
    std::map<std::string, std::set<std::string>> follow = first_and_follow.get_follow();

    // Generate parsing table
    auto parsingTable = ParsingTable::parsing_table_derivation(grammar, terminals, non_terminals, first, follow);

    // Print parsing table for verification
    for (const auto &non_terminal_entry: parsingTable) {
        std::cout << "Non-terminal: " << non_terminal_entry.first << "\n";
        for (const auto &terminal_entry: non_terminal_entry.second) {
            std::cout << "  Terminal: " << terminal_entry.first << " -> [ ";
            for (const auto &symbol: terminal_entry.second) {
                std::cout << symbol << " ";
            }
            std::cout << "]\n";
        }
    }
}
void test3() {
    // Define grammar
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> grammar = {
            {"bexpr", {{"bexpr", "or", "bterm"},{"bterm"}}},
            {"bterm", {{"bterm", "and", "bfactor"},{"bfactor"}}},
            {"bfactor",{{"not","bfactor"},{"(","bexpr",")"},{"true"},{"false"}}}
    };

    // Define terminals and non-terminals
    std::set<std::string> terminals = {"or", "and","not" ,"(", ")", "true","false", EPSILON};
    std::set<std::string> non_terminals = {"bexpr", "bterm","bfactor"};
    std::vector<std::string> order_non_terminals = {"bexpr", "bterm","bfactor"};

//    Apply Left Recursion and Left Factoring
    HandleLeftRecursion left_recursion(grammar,non_terminals,order_non_terminals);
    left_recursion.eliminate_left_recursion();
    grammar = left_recursion.getGrammar();
    non_terminals = left_recursion.getNonTerminals();

    print_grammar(grammar);

    HandleLeftFactoring left_factoring(grammar);
    left_factoring.eliminate_left_factoring();
    grammar = left_factoring.getGrammar();


//    Create first and follow
    FirstAndFollow first_and_follow(grammar,terminals,non_terminals,"bexpr");
    first_and_follow.create_first_and_follow();
    std::map<std::string, std::set<std::string>> first = first_and_follow.get_first();
    std::map<std::string, std::set<std::string>> follow = first_and_follow.get_follow();

    // Generate parsing table
    auto parsingTable = ParsingTable::parsing_table_derivation(grammar, terminals, non_terminals, first, follow);

    // Print parsing table for verification
    for (const auto &non_terminal_entry: parsingTable) {
        std::cout << "Non-terminal: " << non_terminal_entry.first << "\n";
        for (const auto &terminal_entry: non_terminal_entry.second) {
            std::cout << "  Terminal: " << terminal_entry.first << " -> [ ";
            for (const auto &symbol: terminal_entry.second) {
                std::cout << symbol << " ";
            }
            std::cout << "]\n";
        }
    }
}

void test4() {
    // Define grammar
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> grammar = {
            {"bexpr", {{"bexpr", "or", "bterm"},{"bterm"}}},
            {"bterm", {{"bterm", "and", "bfactor"},{"bfactor"}}},
            {"bfactor",{{"not","bfactor"},{"(","bexpr",")"},{"true"},{"false"}}}
    };

    // Define terminals and non-terminals
    std::set<std::string> terminals = {"or", "and","not" ,"(", ")", "true","false", EPSILON};
    std::set<std::string> non_terminals = {"bexpr", "bterm","bfactor"};
    std::vector<std::string> order_non_terminals = {"bexpr", "bterm","bfactor"};

//    Apply Left Recursion and Left Factoring
    HandleLeftRecursion left_recursion(grammar,non_terminals,order_non_terminals);
    left_recursion.eliminate_left_recursion();
    grammar = left_recursion.getGrammar();
    non_terminals = left_recursion.getNonTerminals();

    print_grammar(grammar);

    HandleLeftFactoring left_factoring(grammar);
    left_factoring.eliminate_left_factoring();
    grammar = left_factoring.getGrammar();


//    Create first and follow
    FirstAndFollow first_and_follow(grammar,terminals,non_terminals,"bexpr");
    first_and_follow.create_first_and_follow();
    std::map<std::string, std::set<std::string>> first = first_and_follow.get_first();
    std::map<std::string, std::set<std::string>> follow = first_and_follow.get_follow();

    // Generate parsing table
    auto parsingTable = ParsingTable::parsing_table_derivation(grammar, terminals, non_terminals, first, follow);

    // Print parsing table for verification
    for (const auto &non_terminal_entry: parsingTable) {
        std::cout << "Non-terminal: " << non_terminal_entry.first << "\n";
        for (const auto &terminal_entry: non_terminal_entry.second) {
            std::cout << "  Terminal: " << terminal_entry.first << " -> [ ";
            for (const auto &symbol: terminal_entry.second) {
                std::cout << symbol << " ";
            }
            std::cout << "]\n";
        }
    }
}

int main() {
//    test1();
//    test2();
    test3();
    return 0;
}
