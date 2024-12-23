#include "HandleLeftFactoring.h"

using namespace std;
HandleLeftFactoring::HandleLeftFactoring(
        std::unordered_map<std::string, std::vector<std::vector<std::string>>> grammar) : grammar(std::move(grammar)) {}

std::vector<std::string> HandleLeftFactoring::find_longest_common_prefix(std::vector<std::vector<std::string>> productions) {
    if(productions.size()<=1) return {};


    sort(productions.begin(),productions.end());

    size_t common_len=0;
    vector<string> common_prefix = productions[0];
    for(size_t i=1 ; i< productions.size() ; i++){
        size_t j = 0;
        vector<string> current_production = productions[i];
        while(common_prefix[j]==current_production[j] && j<min(common_prefix.size(),current_production.size()) ){
            j++;
        }
        if(common_len==0 && j==0){
            common_prefix = productions[i];
        }
        else if(j>=common_len){
            common_len = j;
        }
        else{
            break;
        }
    }

    return vector<string>(common_prefix.begin(),common_prefix.begin()+common_len);
}

void HandleLeftFactoring::eliminate_left_factoring() {
    unordered_map<string, vector<vector<string>>> new_grammar;
    for(auto[non_terminal,productions]:grammar){
        vector<vector<string>> remaining_productions = productions;
        int cnt = 1;
        while(true){
            vector<string> common_prefix = find_longest_common_prefix(remaining_productions);
            if(common_prefix.size() < 1) break;

            string new_non_terminal = non_terminal + "`" + to_string(cnt++);
            vector<vector<string>> common_productions;
            vector<vector<string>> non_common_productions;
            for(auto production:remaining_productions){
                if(equal(common_prefix.begin(),common_prefix.end(),production.begin())){
                    if(common_prefix.size()==production.size()){
                        common_productions.push_back({EPSILON});
                    }else {
                        common_productions.push_back(vector<string>(production.begin()+common_prefix.size(),production.end()));
                    }
                }
                else{
                    non_common_productions.push_back(production);
                }
            }
            new_grammar[new_non_terminal] = common_productions;
            common_prefix.push_back(new_non_terminal);
            new_grammar[non_terminal].push_back(common_prefix);

            remaining_productions = non_common_productions;
        }
        new_grammar[non_terminal].insert(new_grammar[non_terminal].end(),remaining_productions.begin(),remaining_productions.end());
    }
    grammar = new_grammar;
}

std::unordered_map<std::string, std::vector<std::vector<std::string>>> HandleLeftFactoring::getGrammar() {
    return grammar;
}
