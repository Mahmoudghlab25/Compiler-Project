#include <iostream>
#include "ParsingInterface/ParsingInterface.h"
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
    const char * parser_input_path =R"(D:\compiler_phase2_clion\Phase2\Input\parser_input.txt)";
    const char *rules_input_path =R"(D:\compiler_phase2_clion\Phase2\Input\rules.txt)";
    const char *program_input_path =R"(D:\compiler_phase2_clion\Phase2\Input\program_input.txt)";
    const string output_path =R"(D:\compiler_phase2_clion\Phase2\Output)";

    ParsingInterface p(parser_input_path,rules_input_path,program_input_path,output_path);

    p.compute_parser();
    return 0;
}

