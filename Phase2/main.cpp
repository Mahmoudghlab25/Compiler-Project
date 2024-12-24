#include <iostream>
#include "ParserRules/ParserRulesHandler.h"
#include "../Phase1/FileReader.cpp"

using namespace std;

int main() {
    ParserRulesHandler p;
    FileReader reader;
    string path = R"(D:\compiler_phase2_clion\Phase2\parser_input.txt)";
    auto lines = reader.readLines(path);
    p.parseRules(lines);
}

