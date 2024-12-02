#include <iostream>
#include "common.h"
#include "LexicalRuleParser.h"

using namespace std;

int main()
{
    string rhs = "abc";
    unordered_set<string> definedVars = { {"digit"} };
    auto parser = LexicalRuleParser(rhs, definedVars);
    auto tokens = parser.parse();
    for (auto& token : tokens) {
        cout << token.type << " " << token.value << endl;
    }

    /*::testing::InitGoogleTest();
    return RUN_ALL_TESTS();*/
}