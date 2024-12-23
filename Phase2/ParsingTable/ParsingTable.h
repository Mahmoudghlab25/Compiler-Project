#ifndef COMPILER_PROJECT_PARSINGTABLE_H
#define COMPILER_PROJECT_PARSINGTABLE_H

#include <iostream>
#include <set>
#include <map>
#include <vector>
#include <unordered_map>

class ParsingTable {
private:
    std::set<std::string> terminals;
public:
    ParsingTable();
    static std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::string>>>
    parsing_table_derivation(std::map<std::string, std::vector<std::vector<std::string>>> grammar,
                             const std::set<std::string> &terminals,
                             const std::set<std::string> &non_terminals,
                             std::map<std::string, std::set<std::string>> first,
                             std::map<std::string, std::set<std::string>> follow);
};

#endif //COMPILER_PROJECT_PARSINGTABLE_H
