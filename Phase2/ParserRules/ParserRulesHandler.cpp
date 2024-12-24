#include "ParserRulesHandler.h"

using namespace std;

Grammar ParserRulesHandler::parseRules(const vector<string>& parserRules) {
    LexicalRulesHandler lexRulesHandler;
    // rules for extracting tokens from defined parser rules
    vector<string> rules = {
        "letter = a-z|A-Z",
        "digit = 0-9",
        "symbol = !|\"|#|$|%|&|'|\\(|\\)|\\*|\\+|,|\\-|.|/|\\:|;|<|\\=|>|?|@|\\[|\\\\|\\]|^|_|`|\\{|\\||\\}|~",
        "start: #",
        "nonterminal: letter(letter|digit|_)*",
        "terminal: '(symbol|letter|digit)+'",
        "assign: \\:\\:\\=",
        "or: \\|",
        "lambda: \\L"
    };
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
    LexicalAnalyzer lexer(parserRules, minStates);
    vector<string> tokens = lexer.analyze();

    for (auto& token: tokens) {
        cout << token << "\n";
    }
    cout << endl;
    return {};
}