#ifndef COMPILER_PROJECT_HANDLELEFTFACTORING_H
#define COMPILER_PROJECT_HANDLELEFTFACTORING_H
#include <iostream>
#include <bits/stdc++.h>
#include "../Symbols.h"

class HandleLeftFactoring{
private:
    std::unordered_map<std::string , std::vector<std::vector<std::string >>> grammar;
    std::set<std::string> non_terminals;
public:
    std::vector<std::string> find_longest_common_prefix(std::vector<std::vector<std::string>> productions);
    HandleLeftFactoring(std::unordered_map<std::string, std::vector<std::vector<std::string>>> grammar,std::set<std::string> non_terminals);
    void eliminate_left_factoring();

    std::unordered_map<std::string, std::vector<std::vector<std::string>>> getGrammar();

    std::set<std::string> getNonTerminals();
};

#endif //COMPILER_PROJECT_HANDLELEFTFACTORING_H
