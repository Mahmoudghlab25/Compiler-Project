#include "HandleLeftRecursion.h"
using namespace std;
HandleLeftRecursion::HandleLeftRecursion(
        unordered_map<std::string, std::vector<std::vector<std::string>>> &grammar,
        set<std::string> &nonTerminals,
        std::vector<std::string> &order_non_terminals) :
        grammar(grammar),
        non_terminals(nonTerminals),
        order_non_terminals(order_non_terminals){}


void HandleLeftRecursion::eliminate_direct_left_recursion(string non_terminal, vector<vector<string>> &productions) {
    vector<vector<string>> recursive, non_recursive;

    for(auto& production: productions){
        if(production[0]==non_terminal){
            recursive.push_back(vector<string>(production.begin()+1,production.end()));
        }
        else{
            non_recursive.push_back(production);
        }
    }

    if(non_recursive.size()==productions.size()) return;

    string new_non_terminal = non_terminal + "`";
    for(auto& production:recursive){
        production.push_back(new_non_terminal);
    }

    if(non_recursive.size() == 0){
        non_recursive.push_back(vector<string>{new_non_terminal});
    }
    else {
        for (auto &production: non_recursive) {
            if (production[0] == EPSILON) {
                production[0] = new_non_terminal;
            } else production.push_back(new_non_terminal);
        }
    }

    recursive.push_back(vector<string>{EPSILON});
    grammar[new_non_terminal] = recursive;

    grammar[non_terminal] = non_recursive;

    non_terminals.insert(new_non_terminal);
}

void HandleLeftRecursion::eliminate_left_recursion() {
    auto i=order_non_terminals.begin();
    for(auto A: order_non_terminals){
        auto productions_A = grammar[A];
        for(auto B :vector<string>(order_non_terminals.begin(),i)){
            auto productions_B = grammar[B];
            if(A==B)continue;

            vector<vector<string>> new_productions_A;
            for(auto& production_A:productions_A){
                if(production_A[0]==B){
                    for(auto& production_B:productions_B){
                        vector<string> new_production_B = production_B;
                        if(new_production_B[0] == EPSILON){
                            new_production_B = vector<string>(production_A.begin()+1,production_A.end());
                        }
                        else {
                            new_production_B.insert(new_production_B.end(),
                                                    production_A.begin() + 1,
                                                    production_A.end());
                        }
                        new_productions_A.push_back(new_production_B);
                    }
                }
                else {
                    new_productions_A.push_back(production_A);
                }
            }
            grammar[A] = new_productions_A;
        }
        eliminate_direct_left_recursion(A,grammar[A]);
        i++;
    }
}

unordered_map<std::string, std::vector<std::vector<std::string>>> HandleLeftRecursion::getGrammar(){
    return grammar;
}

set<std::string> HandleLeftRecursion::getNonTerminals(){
    return non_terminals;
}
