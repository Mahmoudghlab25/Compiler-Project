#include "FirstAndFollow.h"
using namespace std;

FirstAndFollow::FirstAndFollow(
        unordered_map<string,vector<vector<string>>> grammar,
        set<string> terminals,
        set<string> non_terminals,
        string start_non_terminal
):
grammar(grammar),
terminals(terminals),
non_terminals(non_terminals),
start_non_terminal(start_non_terminal)
{}

set<string> FirstAndFollow::compute_non_terminal_first(string non_terminal){
    if(!first[non_terminal].empty()){
        return first[non_terminal];
    }


    set<string> result;
    vector<vector<string>> productions = grammar[non_terminal];
    for(auto production:productions){
        int cnt = 0;
        for(auto element : production) {
            if (is_terminal(element)) {
                result.insert(element);
                break;
            } else {
                set<string> non_terminal_first = compute_non_terminal_first(element);
                bool temp = false;
                for(auto e : non_terminal_first){
                    if(e != EPSILON) result.insert(e);
                    else temp = true;
                }
                if(!temp) break;
                else cnt++;
            }
        }
        if(cnt==production.size()) result.insert(EPSILON);
    }
    return result;
}

void FirstAndFollow::create_first() {
    for(auto non_terminal : non_terminals){
        first[non_terminal] = compute_non_terminal_first(non_terminal);
    }
}

unordered_set<string> visited; // Track visited non-terminals in the current recursion
void FirstAndFollow::compute_non_terminal_follow(string non_terminal){
    if (visited.find(non_terminal) != visited.end()) {
        return; // Prevent infinite recursion
    }
    visited.insert(non_terminal);

    for(auto [lhs, productions]:grammar){
        for(auto production:productions){
            for(int i=0 ; i<production.size() ; i++){
                if(production[i]==non_terminal){
                    if(i<production.size()-1){
                        if(is_terminal(production[i+1])){
                            follow[non_terminal].insert(production[i+1]);
                        }
                        else{
                            for(int j=i+1 ; j<production.size() ; j++) {
                                follow[non_terminal].insert(first[production[j]].begin(), first[production[j]].end());
                                follow[non_terminal].erase(EPSILON);
                                if (first[production[j]].find(EPSILON) == first[production[j]].end()) {
                                    break;
                                }
                                if(j==production.size()-1){
                                    compute_non_terminal_follow(lhs);
                                    follow[non_terminal].insert(follow[lhs].begin(),follow[lhs].end());
                                }
                            }
                        }
                    }
                    else{
                        compute_non_terminal_follow(lhs);
                        follow[non_terminal].insert(follow[lhs].begin(),follow[lhs].end());
                    }
                }
            }
        }
    }
}

void FirstAndFollow::create_follow() {
    follow[start_non_terminal].insert("$");
    for(auto non_terminal : non_terminals){
        compute_non_terminal_follow(non_terminal);
        visited.clear();
    }
}

void FirstAndFollow::create_first_and_follow() {
    create_first();
    create_follow();
}

map<string, set<string>> FirstAndFollow::get_first() {
    return first;
}

map<string, set<string>> FirstAndFollow::get_follow() {
    return follow;
}

bool FirstAndFollow::is_terminal(string terminal) {
    return terminals.find(terminal) != terminals.end();
}