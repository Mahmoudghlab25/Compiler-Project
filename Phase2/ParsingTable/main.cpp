//
// Created by Mahmo on 12/22/2024.
//
#include <iostream>
#include <set>
#include <map>
#include <vector>
#include "ParsingTable.h"

int main() {
    // Define grammar
    std::map<std::string, std::vector<std::vector<std::string>>> grammar = {
            {"S", {{"R", "T"}}},
            {"R", {{"s", "U", "R", "b"}, {"λ"}}},
            {"U", {{"u", "U"},           {"λ"}}},
            {"V", {{"v", "V"},           {"λ"}}},
            {"T", {{"V", "t", "T"},      {"λ"}}}
    };

    // Define terminals and non-terminals
    std::set<std::string> terminals = {"s", "u", "v", "t", "b"};
    std::set<std::string> non_terminals = {"S", "R", "U", "V", "T"};

    // Define first sets
    std::map<std::string, std::set<std::string>> first = {
            {"S", {"s", "v", "t", "λ"}},
            {"A", {"s","λ"}},
            {"B", {"u","λ"}},
            {"a", {"v","λ"}},
            {"b", {"b"}},
            {"c", {"c"}}
    };

    // Define follow sets
    std::map<std::string, std::set<std::string>> follow = {
            {"S", {"$", "b"}},
            {"A", {"b"}},
            {"B", {"$"}}
    };

    // Create ParsingTable object
    ParsingTable table;

    // Generate parsing table
    auto parsingTable = table.parsing_table_derivation(grammar, terminals, non_terminals, first, follow);

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

    return 0;
}
