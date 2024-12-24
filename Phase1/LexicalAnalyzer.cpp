#include <iostream>
#include <utility>
#include "State.h"
#include "MinimizeDFA/MinimizeDFA.h"
#include "FileReader.cpp"
#include "FileWriter.cpp"
#include "LexicalRulesHandler.h"

class LexicalAnalyzer {
private:
    std::vector<std::string> codelines;
    std::set<State *> states;
public:
    explicit LexicalAnalyzer(std::vector<std::string> codelines, std::set<State *> states) {
        this->codelines = std::move(codelines);
        this->states = std::move(states);
    }

    std::vector<std::string> analyze() {
        State *currentState = *(this->states.begin());
        std::string token;
        std::vector<std::string> tokens;
        std::string currentAccToken;
        int index;
        int errIndex;
        int lastAccInd = 0;
        if (currentState->is_dead_state()) {
            return tokens; // Return early if the starting state is dead
        }

        for (auto &codeline: this->codelines) {
            index = 0;
            errIndex = 0;
            token.clear();
            currentAccToken.clear();
            currentState = *(this->states.begin());
            while (index < codeline.size()) {
                cout << index << endl;
                if (currentState->is_dead_state()) {
                    if (!token.empty()) {
//                        cout << "index: " << index << ", Token: " << token << endl;
                        tokens.push_back(currentAccToken);
                        tokens.push_back(token);
                    } else {
//                        cout << "index: " << index << ", " << "ERRRRRRRRR" << endl;
                        errIndex++;
                        index = errIndex;
                        tokens.emplace_back("Error at index " + std::to_string(index));
                    }
                    currentState = *(this->states.begin());
                    token.clear();
                    currentAccToken.clear();
                    continue;
                }
                if (currentState->is_accepting_state()) {
                    token = currentState->get_token_type();
                    lastAccInd = index - 1;
                    errIndex = index;
                }
                if (codeline[index] == ' ') {
                    if (!token.empty()) {
//                        cout << "index: " << index << ", Token: " << token << endl;
                        tokens.push_back(currentAccToken);
                        tokens.push_back(token);
                    }
                    currentState = *(this->states.begin());
                    token.clear();
                    currentAccToken.clear();
                    index++;
                    continue;
                }
                if (currentState->get_transitions().count(codeline[index]) > 0) {
                    currentState = *(currentState->get_transitions().at(codeline[index]).begin());
                    if (currentState->is_dead_state()) {
                        index = lastAccInd;
                    } else {
                        currentAccToken += codeline[index];
                    }
                } else {
                    if (!token.empty()) {
                        tokens.push_back(currentAccToken);
                        tokens.push_back(token);
                    }
                    cerr << "Error: Character " << codeline[index] << " isn't in language." << endl;
                    string err;
                    err += codeline[index];
                    tokens.push_back(err);
                    tokens.emplace_back("Error");
                    currentState = *(this->states.begin());
                    token.clear();
                    currentAccToken.clear();
                }
                index++;
            }
            if (currentState->is_accepting_state()) {
                tokens.push_back(currentAccToken);
                tokens.push_back(currentState->get_token_type());
            }
        }

        return tokens;
    }

    //abcfda
    std::vector<std::string> Analyze() {
        State *currentState = *(this->states).begin();//s0
        std::string token;//""
        std::vector<std::string> tokens;//{}
        int index = 0;//
        int stIndex = 0;//
        int lastAccIndex = -1;//
        if (currentState->is_dead_state()) {
            return tokens;
        }
        for (auto &codeline: this->codelines) {
            index = 0;
            token.clear();//""
            stIndex = 0;
            lastAccIndex = -1;
            currentState = *(this->states).begin();//abcfda
            while (index < codeline.size()) {//a
                if (currentState->get_transitions().count(codeline[index]) > 0) {
                    currentState = *(currentState->get_transitions().at(codeline[index])).begin();
                }
                //dead
                if (currentState->is_dead_state() || codeline[index] == ' ') {
                    //err
                    if (lastAccIndex == -1) {
                        string err = codeline.substr(stIndex, index - stIndex + 1);
                        tokens.emplace_back(trim(err));
                        if (codeline[index] != ' ') {
                            tokens.emplace_back("Error");
                        }
                        stIndex = index;
                        currentState = *(this->states).begin();
                    }
                        //last token
                    else {
                        string acc = codeline.substr(stIndex, lastAccIndex - stIndex + 1);
                        tokens.emplace_back(trim(acc));
                        tokens.push_back(token);
                        token.clear();
                        index = lastAccIndex + 1;
                        lastAccIndex = -1;
                        stIndex = index;
                        currentState = *(this->states).begin();
                        continue;
                    }
                }
                    //Acc
                else if (currentState->is_accepting_state()) {
                    lastAccIndex = index;
                    token = currentState->get_token_type();
                }
                // else it is normal; continue
                index++;
            }
            if (currentState->is_accepting_state()) {
                string acc = codeline.substr(stIndex, codeline.size() - stIndex);
                tokens.emplace_back(trim(acc));
                tokens.push_back(currentState->get_token_type());
            } else {
                string err = codeline.substr(stIndex, codeline.size() - stIndex);
                tokens.emplace_back(trim(err));
                if (codeline[index] != ' ') {
                    tokens.emplace_back("Error");
                }
            }
        }
        return tokens;
    }
};

//int main() {
//    // Define all states
//    State *q_start = new State(0, false, "");
//    State *q_int = new State(1, true, "int");
//    State *q_while = new State(2, true, "while");
//    State *q_identifier = new State(3, true, "identifier");
//    State *q_assignment = new State(4, true, "=");
//    State *q_not_equal = new State(5, true, "!=");
//    State *q_addition = new State(6, true, "+");
//    State *q_number = new State(7, true, "number");
//    State *q_comma = new State(8, true, ",");
//    State *q_semicolon = new State(9, true, ";");
//    State *q_open_paren = new State(10, true, "(");
//    State *q_close_paren = new State(11, true, ")");
//    State *q_open_brace = new State(12, true, "{");
//    State *q_close_brace = new State(13, true, "}");
//    State *q_dead = new State(14, false, "dead");
//
//// Transitions for `int`
//    q_start->add_transition('i', q_int);
//    q_int->add_transition('n', q_int);
//    q_int->add_transition('t', q_int);
//
//// Transitions for `while`
//    q_start->add_transition('w', q_while);
//    q_while->add_transition('h', q_while);
//    q_while->add_transition('i', q_while);
//    q_while->add_transition('l', q_while);
//    q_while->add_transition('e', q_while);
//
//// Transitions for identifiers (e.g., `sum`, `count`, etc.)
//    for (char c = 'a'; c <= 'z'; c++) {
//        q_start->add_transition(c, q_identifier);
//        q_identifier->add_transition(c, q_identifier);
//    }
//    for (char c = 'A'; c <= 'Z'; c++) {
//        q_start->add_transition(c, q_identifier);
//        q_identifier->add_transition(c, q_identifier);
//    }
//    for (char c = '0'; c <= '9'; c++) {
//        q_identifier->add_transition(c, q_identifier);
//    }
//    q_identifier->add_transition('_', q_identifier);
//
//// Transitions for digits (e.g., `10`, `1`)
//    for (char c = '0'; c <= '9'; c++) {
//        q_start->add_transition(c, q_number);
//        q_number->add_transition(c, q_number);
//    }
//
//// Transitions for `=` and `!=`
//    q_start->add_transition('=', q_assignment);
//    q_start->add_transition('!', q_not_equal);
//    q_not_equal->add_transition('=', q_not_equal);
//
//// Transitions for `+`
//    q_start->add_transition('+', q_addition);
//
//// Transitions for delimiters
//    q_start->add_transition(',', q_comma);
//    q_start->add_transition(';', q_semicolon);
//    q_start->add_transition('(', q_open_paren);
//    q_start->add_transition(')', q_close_paren);
//    q_start->add_transition('{', q_open_brace);
//    q_start->add_transition('}', q_close_brace);
//
//// Dead state for invalid transitions
//    for (char c = 0; c < 128; c++) {
//        q_start->add_transition(c, q_dead);
//        q_dead->add_transition(c, q_dead);
//    }
//
//// Link everything into the DFA
//    std::set<State *> dfa = {q_start, q_int, q_while, q_identifier, q_assignment, q_not_equal,
//                             q_addition, q_number, q_comma, q_semicolon, q_open_paren,
//                             q_close_paren, q_open_brace, q_close_brace, q_dead};
//
//    std::set<char> alphabet = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
//                               'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
//                               'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
//                               'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
//                               '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '=', '!', '+',
//                               ',', ';', '(', ')', '{', '}', '_'};
//
//     std::vector<std::unordered_map<char, int>> transitionTable = {};
////    MinimizeDFA minimizer;
////    std::set<State *> minimized_dfa = minimizer.minDFA(dfa, transitionTable, alphabet);
////    std::cout << "Minimized DFA States:\n";
////    for (State *state: dfa) {
////        state->print_state();
////    }
//
//    //Change it with your current path.
//    std::string path = R"(C:\Users\Mahmo\OneDrive - Alexandria University\Documents\GitHub\Compiler-Project\Phase1\Input\)";
//    FileReader fileReader(path + "lecture test.txt");
//    std::vector<std::string> codeLines = fileReader.readLines();
//
//    LexicalAnalyzer lexicalAnalyzer(codeLines, dfa);
//    std::vector<std::string> tks = lexicalAnalyzer.analyze();
//
//    FileWriter fileWriter("lecture test");
//    fileWriter.writeLines(tks);
//
//
//    return 0;
//}