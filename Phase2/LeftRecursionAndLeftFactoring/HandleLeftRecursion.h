#ifndef COMPILER_PROJECT_HANDLELEFTRECURSION_H
#define COMPILER_PROJECT_HANDLELEFTRECURSION_H
#include <iostream>
#include <bits/stdc++.h>
#include "../Symbols.h"
class HandleLeftRecursion{
private:
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> grammar;
    std::vector<std::string> order_non_terminals;
    std::set<std::string> non_terminals;
    void eliminate_direct_left_recursion(std::string non_terminal, std::vector<std::vector<std::string>> &productions);
public:
    HandleLeftRecursion(std::unordered_map<std::string, std::vector<std::vector<std::string>>> &grammar,
                        std::set<std::string> &nonTerminals,
                        std::vector<std::string> &order_non_terminals);
    void eliminate_left_recursion();

    std::unordered_map<std::string, std::vector<std::vector<std::string>>> getGrammar();

    std::set<std::string> getNonTerminals();
};
#endif //COMPILER_PROJECT_HANDLELEFTRECURSION_H
