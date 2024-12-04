#include <iostream>
#include "common.h"
#include "LexicalRuleParser.h"
#include "LexicalRulesHandler.h"
#include "NFA.h"

using namespace std;

int main()
{
    string rhs = "a-z";
    // never pass an rvalue to an lvalue reference
    //auto parser = LexicalRuleParser(rhs, definedVars);
    /*auto parser = LexicalRuleParser(rhs, unordered_set<string> {});
    auto tokens = parser.parse();
    for (auto& token : tokens) {
        cout << token.type << " " << token.value << endl;
    }*/

    /*NFA* n = new NFA();
    n = n->basic('@');

    n->display();

    cout << "********************************\n";

    NFA clone = NFA(*n);
    clone.display();*/
    /*NFA* nfa1 = new NFA();
    NFA* nfa2 = new NFA();
    NFA* nfa3 = new NFA();
    nfa3 = nfa3->Concatenate(*nfa1->basic('a'), *nfa2->basic('b'));
    nfa3->display();*/

    //LexicalRulesHandler handler;

    //handler.extractStatements(
    //    // char=0-9|a-c
    //    vector<string> {
    //        //string("char=(ab)*")
    //        /*string("char=a|b*")*/
    //        /*string("char=(a|b)*")*/
    //        /*string("char=(a-c)*")*/
    //        /*string("char=(a)((b))")*/
    //        /*string("var=(\\L|a)*")*/
    //        /*string("var=((0|1)+(0|1)(0|1))")*/
    //    string("letter=a|b"),
    //        string("letters:letter+")
    //    // combine vars defined by : only --> error, output
    //    // read keywords {}, punc [] --> done
    //    // accept input file, convert to vector<string> --> done
    //    // push changes
    //    // refactor nfa methods to functions
    //    // continue testing
    //    }
    //);

    //handler.generateNFAs()->display();


    LexicalRulesHandler handler;
    auto rules = handler.readRules("rules.txt");
    rules.pop_back();
    handler.extractStatements(rules);
    handler.extractKeywords(rules);
    handler.extractPunctuation(rules);
    handler.generateNFAs()->display();
}