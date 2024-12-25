#include "ParsingInterface.h"
#include "../stack_parser/stack_parser.h"


using namespace std;

ParsingInterface::ParsingInterface(const char *parserInput, const char *rulesInput, const char *programInput, string output_path)
        : parser_input(parserInput), rules_input(rulesInput), program_input(programInput), output_path(output_path) {}

void ParsingInterface::compute_min_states() {
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
    min_states = minDFA.minDFA(states, nfAtoDfaConverter.get_dfa_transition_table(), alphas);

    map<int, std::unordered_map<char, int>> minTransitionTable = MinimizeDFA::getReducedTransitionTable(min_states);
}

void ParsingInterface::compute_grammar() {
    ParserRulesHandler p;
    FileReader reader;
    auto lines = reader.readLines(parser_input);
    auto grammarParser = p.parseRules(lines);

    grammar = grammarParser.grammar;
    terminals = grammarParser.terminals;
    non_terminals = grammarParser.nonTerminals;
    order_non_terminals = grammarParser.NonTerminalsOrder;
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

    auto grammar_output_path = output_path + ("grammar.md");
    file_writer.writeGrammarToMarkdown(grammar,grammar_output_path);
}

void ParsingInterface::compute_first_and_follow_and_predictive_table() {
    FirstAndFollow first_and_follow(grammar, terminals, non_terminals, order_non_terminals[0]);
    first_and_follow.create_first_and_follow();
    first = first_and_follow.get_first();
    follow = first_and_follow.get_follow();
    predictive_table = ParsingTable::parsing_table_derivation(grammar, terminals, non_terminals, first, follow);

    auto first_output_path = output_path + ("first.md");
    file_writer.writeSetToMarkdown(first,first_output_path,"First Set");

    auto follow_output_path = output_path + ("follow.md");
    file_writer.writeSetToMarkdown(follow,follow_output_path,"Follow Set");
}

void ParsingInterface::compute_actions() {
    FileReader fileReader;
    vector<string> lines = fileReader.readLines(program_input);

    LexicalAnalyzer lexicalAnalyzer(lines, min_states);

    stack_parser stackParser(predictive_table,order_non_terminals[0],lexicalAnalyzer,output_path);
    stackParser.parse(terminals);
}

void ParsingInterface::compute_parser() {
    compute_min_states();
    compute_grammar();
    compute_left_recursion_and_factoring();
    compute_first_and_follow_and_predictive_table();
    compute_actions();
}
