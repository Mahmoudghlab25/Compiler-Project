#include <iostream>
#include "LexicalRulesHandler.h"
#include "NFA.h"
#include "DFAConverter.h"
#include "MinimizeDFA/MinimizeDFA.h"
#include "LexicalAnalyzer.cpp"

using namespace std;

int main() {
    LexicalRulesHandler handler;
    auto rules = handler.readRules(
            R"(C:\Users\Mahmo\OneDrive - Alexandria University\Documents\GitHub\Compiler-Project\Phase1\rules.txt)");
    rules.pop_back();
    handler.extractKeywords(rules);
    handler.extractPunctuation(rules);
    handler.extractStatements(rules);
    cout << "---------------------------------------------------" << endl << "DFA:" << endl;
    NFA *nfa = (handler.generateNFAs());
    set<char> alphas = handler.getAlphabet();
    NFAtoDFAConverter nfAtoDfaConverter(*nfa, alphas);
    nfAtoDfaConverter.create_DFA();
    vector<State *> states = nfAtoDfaConverter.get_dfa_states();
    cout << "Number of DFA states: " << states.size() << endl;
    cout << "---------------------------------------------------" << endl << "MinDFA:" << endl;
    MinimizeDFA minDFA;
    set<State *> minStates = minDFA.minDFA(states, nfAtoDfaConverter.get_dfa_transition_table(), alphas);
    cout << "Number of min DFA states: " << minStates.size() << endl;
    cout << "--------------------------------------------------" << endl << "Minimized Transition Table:" << endl;
    map<int, std::unordered_map<char, int>> minTransitionTable = MinimizeDFA::getReducedTransitionTable(minStates);
    for (const auto &stTable: minTransitionTable) {
        cout << "State number " << stTable.first << endl;
        for (auto pair: stTable.second) {
            cout << "character: " << pair.first << ", Target state id: " << pair.second << endl;
        }
    }
    FileReader fileReader;
    string path = R"(C:\Users\Mahmo\OneDrive - Alexandria University\Documents\GitHub\Compiler-Project\Phase1\Input\test1.txt)";
    vector<string> lines = fileReader.readLines(path);

    LexicalAnalyzer lexicalAnalyzer(lines, minStates);
    FileWriter fileWriter(
            R"(test)");
    vector<string> tokens = lexicalAnalyzer.analyze();
    fileWriter.writeLines(tokens);
}