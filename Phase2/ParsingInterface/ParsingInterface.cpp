#include "ParsingInterface.h"
#include "../stack_parser/stack_parser.h"


using namespace std;

ParsingInterface::ParsingInterface(const char *parserInput, const char *rulesInput, const char *programInput, string output_path)
        : parser_input(parserInput), rules_input(rulesInput), program_input(programInput), output_path(output_path) {}

std::vector<std::string> ParsingInterface::get_program_tokens() {
    LexicalRulesHandler handler;
    auto rules = handler.readRules(rules_input);
    rules.pop_back();
    handler.extractKeywords(rules);
    handler.extractPunctuation(rules);
    handler.extractStatements(rules);

    NFA *nfa = (handler.generateNFAs());
    set<char> alphas = handler.getAlphabet();
    NFAtoDFAConverter nfAtoDfaConverter(*nfa, alphas);
    nfAtoDfaConverter.create_DFA();

    vector<State *> states = nfAtoDfaConverter.get_dfa_states();
    MinimizeDFA minDFA;
    set<State *> minStates = minDFA.minDFA(states, nfAtoDfaConverter.get_dfa_transition_table(), alphas);

    map<int, std::unordered_map<char, int>> minTransitionTable = MinimizeDFA::getReducedTransitionTable(minStates);

    FileReader fileReader;
    vector<string> lines = fileReader.readLines(program_input);

    LexicalAnalyzer lexicalAnalyzer(lines, minStates);

    vector<string> tokens = lexicalAnalyzer.analyze();
    auto tokens_output_path = output_path + ("tokens.txt");
    file_writer.writeLines(tokens,tokens_output_path);

    for(int i=1 ; i<tokens.size() ; i+=2){
        tokens_to_parser.push_back(tokens[i]);
    }
    return tokens_to_parser;
}

//TODO wait parser rules
void ParsingInterface::compute_grammar() {

}

void ParsingInterface::compute_left_recursion_and_factoring() {
    // Apply Left Recursion
    HandleLeftRecursion left_recursion(grammar, non_terminals, order_non_terminals);
    left_recursion.eliminate_left_recursion();
    grammar = left_recursion.getGrammar();
    non_terminals = left_recursion.getNonTerminals();

    // Apply Left Factoring
    HandleLeftFactoring left_factoring(grammar,non_terminals);
    left_factoring.eliminate_left_factoring();
    grammar = left_factoring.getGrammar();
    non_terminals = left_factoring.getNonTerminals();

    auto grammar_output_path = output_path + ("grammar.txt");
    file_writer.writeGrammarToMarkdown(grammar,grammar_output_path);
}

void ParsingInterface::compute_first_and_follow_and_predictive_table() {
    FirstAndFollow first_and_follow(grammar, terminals, non_terminals, "METHOD_BODY");
    first_and_follow.create_first_and_follow();
    first = first_and_follow.get_first();
    follow = first_and_follow.get_follow();
    predictive_table = ParsingTable::parsing_table_derivation(grammar, terminals, non_terminals, first, follow);

    auto first_output_path = output_path + ("first.txt");
    file_writer.writeSetToMarkdown(first,first_output_path,"First Set");

    auto follow_output_path = output_path + ("follow.txt");
    file_writer.writeSetToMarkdown(follow,follow_output_path,"Follow Set");
}

void ParsingInterface::compute_parser() {
    get_program_tokens();
    compute_grammar();
    compute_left_recursion_and_factoring();
    compute_first_and_follow_and_predictive_table();

    stack_parser stackParser(predictive_table,order_non_terminals[0],tokens_to_parser);
    stackParser.parse(terminals);
//    stackParser.display_action();

    actions = stackParser.get_actions();
    auto actions_output_path = output_path + ("actions.txt");
    file_writer.writeActionsToMarkdown(actions,actions_output_path);
}
