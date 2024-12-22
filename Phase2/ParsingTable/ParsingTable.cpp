//
// Created by Mahmo on 12/21/2024.
//

#include "ParsingTable.h"

#include <utility>

ParsingTable::ParsingTable() = default;

std::map<std::string, std::map<std::string, std::vector<std::string>>>
ParsingTable::parsing_table_derivation(std::map<std::string, std::vector<std::vector<std::string>>> grammar,
                                       const std::set<std::string> &terminals,
                                       const std::set<std::string> &non_terminals,
                                       std::map<std::string, std::set<std::string>> first,
                                       std::map<std::string, std::set<std::string>> follow) {
    // -1 stands for empty entry, start with -1 initially.
    std::map<std::string, std::map<std::string, std::vector<std::string>>> parsing_table;
    for (const auto &non_terminal: non_terminals) {
        std::vector<std::vector<std::string>> productions = grammar[non_terminal];
        for (const auto &alpha: productions) { // A -> alpha
            std::set<std::string> fst = first[alpha[0]];
            std::map<std::string, std::vector<std::string>> terminal_to_production;
            for (const auto &fir: fst) {
                if (fir.empty()) { // lambda is in first(alpha)
                    for (const auto &fol: follow[non_terminal]) {
                        terminal_to_production.insert(std::make_pair(fol, alpha));
                    }
                } else {
                    terminal_to_production.insert(std::make_pair(fir, alpha));
                }
            }
            parsing_table.insert(std::make_pair(non_terminal, terminal_to_production));
        }
    }
    return parsing_table;
}
