#include <iostream>
#include "LexicalRulesHandler.h"
#include "NFA.h"
#include "DFAConverter.h"
#include "MinimizeDFA/MinimizeDFA.h"
#include "LexicalAnalyzer.cpp"

using namespace std;

int main() {
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
    auto rules = handler.readRules(
            R"(C:\Users\Mahmo\OneDrive - Alexandria University\Documents\GitHub\Compiler-Project\Phase1\rules.txt)");
    rules.pop_back();

    handler.extractStatements(rules);

    handler.extractKeywords(rules);

    handler.extractPunctuation(rules);

//    handler.generateNFAs()->display();
    cout << "---------------------------------------------------" << endl << "DFA:" << endl;
    NFAtoDFAConverter nfAtoDfaConverter(*(handler.generateNFAs()));
    nfAtoDfaConverter.create_DFA();
//    vector<unordered_map<char, int>> trTable = nfAtoDfaConverter.get_dfa_transition_table();
//    int i = 0;
//    for (const auto &table: trTable) {
//        cout << "table index:" << i << endl;
//        i++;
//        for (auto pair: table) {
//            cout << "symbol: " << pair.first << " state id:" << pair.second << endl;
//        }
//    }
    vector<State *> states = nfAtoDfaConverter.get_dfa_states();
    for (const auto &state: states) {
        state->print_state();
    }
    cout << "Number of DFA states: " << states.size() << endl;
    cout << "---------------------------------------------------" << endl << "MinDFA:" << endl;
    MinimizeDFA minDFA;
//    set<char> alphabets = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
//                           'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
//                           '0','1','2','3','4','5','6','7','8','9',
//                           '+','}','{','=',';','<','>','!','(',')','*',',','-','/','.'};
    set<State *> minStates = minDFA.minDFA(states, nfAtoDfaConverter.get_dfa_transition_table());
    for (auto st: minStates) {
        st->print_state();
    }
    cout << "Number of min DFA states: "<<minStates.size() << endl;
    cout << "--------------------------------------------------" << endl << "Minimized Transition Table:" << endl;
//    map<int, std::unordered_map<char, int>> minTransitionTable = MinimizeDFA::getReducedTransitionTable(minStates);
//    for (const auto &stTable: minTransitionTable) {
//        cout << "State number " << stTable.first << endl;
//        for (auto pair: stTable.second) {
//            cout << "character: " << pair.first << ", Target state id: " << pair.second << endl;
//        }
//    }

}