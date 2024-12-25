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
#include "../Phase1/LexicalAnalyzer.cpp"
using namespace std;

class stack_parser{
    private:
       unordered_map<string,unordered_map<string, vector<string>>> parserTable;
       stack<string> parserStack;
       queue<string> returnedStack;
       string startSymbol;
       int index ;
       vector<string>tokens;
       vector<string>prodaction;
       vector<string> actions;
       LexicalAnalyzer lexicalAnalyzer;
       string output_path;
    public:
       stack_parser(unordered_map<string,unordered_map<string, vector<string>>> &parserTable, string &startSymbol, LexicalAnalyzer &lexicalAnalyzer,string &output_path);
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