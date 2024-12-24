#include <iostream>
#include "ParserRules/ParserRulesHandler.h"
#include "../Phase1/FileReader.cpp"

using namespace std;

void print_grammar(std::unordered_map<std::string, std::vector<std::vector<std::string>>> grammar){
    for (const auto& [non_terminal, productions] : grammar) {
        std::cout << non_terminal << " -> ";

        for (size_t i = 0; i < productions.size(); ++i) {
            // Print each production
            for (size_t j = 0; j < productions[i].size(); ++j) {
                std::cout << productions[i][j];
                if (j < productions[i].size() - 1) {
                    std::cout << " ";  // Separate symbols in the production
                }
            }
            if (i < productions.size() - 1) {
                std::cout << " | ";  // Separate productions
            }
        }

        std::cout << std::endl;
    }
}

int main() {
    ParserRulesHandler p;
    FileReader reader;
    string path = R"(D:\compiler_phase2_clion\Phase2\parser_input.txt)";
    auto lines = reader.readLines(path);
    auto grammar = p.parseRules(lines);

    print_grammar(grammar.grammar);


}

