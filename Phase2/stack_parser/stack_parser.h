#ifndef STACK_PARSER_H
#define STACK_PARSER_H
#include <unordered_map>
#include <string>
#include <vector>
#include<stack>
#include <queue>
#include <set>
#include <iostream>
#include "../Symbols.h"
using namespace std;

class stack_parser{
    private:
       unordered_map<string,unordered_map<string, vector<string>>> parserTable;
       stack<string> parserStack;
       queue<string> actions;
       string startSymbol;
       int index ;
       vector<string>tokens;
       vector<string>prodaction;
    public:
       stack_parser(unordered_map<string,unordered_map<string, vector<string>>> parserTable, string startSymbol, vector<string>tokens);
       queue<string> get_actions();
       void parse(
          set<std::string> terminals
       );
       string get_next_token();
       void display_action();
       bool is_terminal(
         std::set<std::string> terminals,
         string in
       );
       string string_stack(stack<string> stk);
       vector<string> get_prodaction();

};

#endif