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
            R"(D:\compiler_phase2_clion\Phase1\rules.txt)");
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
    string path = R"(D:\compiler_phase2_clion\Phase1\input.txt)";
    vector<string> lines = fileReader.readLines(path);

    LexicalAnalyzer lexicalAnalyzer(lines, minStates);
    vector<string> tokens = lexicalAnalyzer.analyze();
    FileWriter fileWriter;
    fileWriter.writeLines(tokens, R"(output.txt)");
}