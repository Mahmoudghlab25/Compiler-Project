#include <iostream>
#include "ParsingInterface/ParsingInterface.h"
using namespace std;

int main() {
    const char * parser_input_path =R"(D:\Level_4_Semester_1\Compilers\Project\Compiler-Project\Phase2\Input\parser_input.txt)";
    const char *rules_input_path =R"(D:\Level_4_Semester_1\Compilers\Project\Compiler-Project\Phase2\Input\rules.txt)";
    const char *program_input_path =R"(D:\Level_4_Semester_1\Compilers\Project\Compiler-Project\Phase2\Input\program_input.txt)";
    const string output_path =R"(D:\Level_4_Semester_1\Compilers\Project\Compiler-Project\Phase2\Output\)";

    ParsingInterface p(parser_input_path,rules_input_path,program_input_path,output_path);

    p.compute_parser();
    return 0;
}

