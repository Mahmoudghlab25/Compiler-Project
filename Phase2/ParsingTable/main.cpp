//
// Created by Mahmo on 12/22/2024.
//
#include <iostream>
#include <set>
#include <map>
#include <vector>
#include "ParsingTable.h"
#include "..\Symbols.h"

void test1() {
    // Define grammar
    std::map<std::string, std::vector<std::vector<std::string>>> grammar = {
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
    std::map<std::string, std::vector<std::vector<std::string>>> grammar = {
            {"E",     {{"T", "Edash"}}},
            {"Edash", {{"+", "T", "Edash"}, {EPSILON}}},
            {"T",     {{"F", "Tdash"}}},
            {"Tdash", {{"*", "F", "Tdash"}, {EPSILON}}},
            {"F",     {{"(", "E", ")"},     {"id"}}}
    };

    // Define terminals and non-terminals
    std::set<std::string> terminals = {"+", "*", "(", ")", "id", EPSILON};
    std::set<std::string> non_terminals = {"E", "Edash", "T", "Tdash", "F"};

    // Define first sets
    std::map<std::string, std::set<std::string>> first = {
            {"E",     {"(", "id"}},
            {"Edash", {"+", EPSILON}},
            {"T",     {"(", "id"}},
            {"Tdash", {"*", EPSILON}},
            {"F",     {"(", "id"}}
    };

    // Define follow sets
    std::map<std::string, std::set<std::string>> follow = {
            {"E",     {"$", ")"}},
            {"Edash", {"$", ")"}},
            {"T",     {"+", "$", ")"}},
            {"Tdash", {"+", "$", ")"}},
            {"F",     {"+", "*", "$", ")"}}
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

int main() {
//    test1();
    test2();
    return 0;
}
