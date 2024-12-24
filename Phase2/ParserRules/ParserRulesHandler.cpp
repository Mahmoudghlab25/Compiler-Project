#include "ParserRulesHandler.h"

using namespace std;

Grammar ParserRulesHandler::parseRules(const vector<string>& parserRules) {
    LexicalRulesHandler lexRulesHandler;
    auto rules = lexRulesHandler.readRules(
            R"(D:\Level_4_Semester_1\Compilers\Project\Compiler-Project\Phase2\rules.txt)");
    rules.pop_back();
    lexRulesHandler.extractKeywords(rules);
    lexRulesHandler.extractPunctuation(rules);
    lexRulesHandler.extractStatements(rules);
    NFA* nfa = lexRulesHandler.generateNFAs();
    set<char> alphas = lexRulesHandler.getAlphabet();
    NFAtoDFAConverter converter(*nfa, alphas);
    converter.create_DFA();
    vector<State*> states = converter.get_dfa_states();
    MinimizeDFA minDFA;
    set<State*> minStates = minDFA.minDFA(
        states,
        converter.get_dfa_transition_table(),
        alphas
    );
    auto minTransitionTable = MinimizeDFA
        ::getReducedTransitionTable(minStates);
    cout<<"Finish Min DFA\n";
    if (minStates.empty()) {
        throw std::runtime_error("minDFA generated an empty states set.");
    }
    LexicalAnalyzer lexer(parserRules, minStates);
    vector<string> tokens = lexer.analyze();

    for (auto& token: tokens) {
        cout << token << "\n";
    }
    cout << endl;
    return {};
}