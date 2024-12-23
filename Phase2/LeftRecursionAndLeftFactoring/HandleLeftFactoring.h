#ifndef COMPILER_PROJECT_HANDLELEFTFACTORING_H
#define COMPILER_PROJECT_HANDLELEFTFACTORING_H
#include <iostream>
#include <bits/stdc++.h>
#include "../Symbols.h"

class HandleLeftFactoring{
private:
    std::unordered_map<std::string , std::vector<std::vector<std::string >>> grammar;
public:
    std::vector<std::string> find_longest_common_prefix(std::vector<std::vector<std::string>> productions);
    explicit HandleLeftFactoring(std::unordered_map<std::string, std::vector<std::vector<std::string>>> grammar);
    void eliminate_left_factoring();

    std::unordered_map<std::string, std::vector<std::vector<std::string>>> getGrammar();
};

#endif //COMPILER_PROJECT_HANDLELEFTFACTORING_H
