#ifndef COMPILER_PROJECT_FIRSTANDFOLLOW_H
#define COMPILER_PROJECT_FIRSTANDFOLLOW_H
#include <iostream>
#include <bits/stdc++.h>

class FirstAndFollow{
private:
    std::map<std::string,std::vector<std::vector<std::string>>> grammar;
    std::set<std::string> terminals;
    std::set<std::string> non_terminals;
    std::map<std::string, std::set<std::string>> first;
    std::map<std::string, std::set<std::string>> follow;
    std::string start_non_terminal;

public:
    FirstAndFollow(
        std::map<std::string,std::vector<std::vector<std::string>>> grammar,
        std::set<std::string> terminals,
        std::set<std::string> non_terminals,
        std::string start_non_terminal
    );
    void create_first_and_follow();
    std::set<std::string> compute_non_terminal_first(std::string non_terminal);
    void create_first();
    void compute_non_terminal_follow(std::string non_terminal);
    void create_follow();
    std::map<std::string, std::set<std::string>> get_first();
    std::map<std::string, std::set<std::string>> get_follow();
    bool is_terminal(std::string terminal);
};

#endif //COMPILER_PROJECT_FIRSTANDFOLLOW_H
