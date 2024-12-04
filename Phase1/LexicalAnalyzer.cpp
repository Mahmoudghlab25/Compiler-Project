#include <iostream>
#include <utility>
#include "State.h"
#include "MinimizeDFA/MinimizeDFA.h"
#include "common.h"
#include "FileReader.cpp"
#include "FileWriter.cpp"

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
        std::string prevAccToken;
        int index = -1;
        if (currentState->is_dead_state()) {
            return tokens;
        }
        for (auto &codeline: this->codelines) {
            while (true) {
                if (currentState->is_dead_state()) {
                    if(!token.empty()){
                        tokens.push_back(token);
                        codeline = codeline.substr(prevAccToken.size());
                    } else{
                        tokens.emplace_back("Error");
                        codeline = codeline.substr(1);
                    }
                    currentState = *(this->states.begin());
                    token = "";
                    currentAccToken = "";
                    prevAccToken = "";
                    index = -1;
                }
                if (currentState->is_accepting_state()) {
                    if (currentAccToken.empty()) {
                        token = currentState->get_token_type();
                    } else if (currentAccToken.size() > prevAccToken.size()) {
                        token = currentState->get_token_type();
                        prevAccToken = currentAccToken;
//                        std::cout<<"token is: "<<token<<std::endl;
                    }
                }
                index++;
                if (index >= codeline.size()) {
                    break;
                }
                currentState = *(currentState->get_transitions().at(codeline[index]).begin());
                currentAccToken += codeline[index];
            }
            index = -1;
        }
        if (currentState->is_accepting_state()) {
            tokens.push_back(token);
        }
        return tokens;
    }

};

//int main() {
//    State *q01411 = new State(0, false, "");
//    State *q2512 = new State(1, false, "");
////    State *q12 = new State(2, true, "id");
//    State *q36 = new State(3, true, "do");
//    State *q7 = new State(4, false, "");
//    State *q8 = new State(5, false, "");
//    State *q9 = new State(6, false, "");
//    State *q10 = new State(7, true, "double");
//    State *qdead = new State(8, false, "dead");
//
//    q01411->add_transition('d', q2512);
//    q01411->add_transition('a', qdead);
//    q01411->add_transition('o', qdead);
//    q01411->add_transition('u', qdead);
//    q01411->add_transition('b', qdead);
//    q01411->add_transition('l', qdead);
//    q01411->add_transition('e', qdead);
//
//    q2512->add_transition('a', qdead);
//    q2512->add_transition('d', qdead);
//    q2512->add_transition('o', q36);
//    q2512->add_transition('u', qdead);
//    q2512->add_transition('b', qdead);
//    q2512->add_transition('l', qdead);
//    q2512->add_transition('e', qdead);
//
////    q12->add_transition('a', qdead);
////    q12->add_transition('d', qdead);
////    q12->add_transition('o', qdead);
////    q12->add_transition('u', qdead);
////    q12->add_transition('b', qdead);
////    q12->add_transition('l', qdead);
////    q12->add_transition('e', qdead);
//
//    q36->add_transition('a', qdead);
//    q36->add_transition('d', qdead);
//    q36->add_transition('o', qdead);
//    q36->add_transition('u', q7);
//    q36->add_transition('b', qdead);
//    q36->add_transition('l', qdead);
//    q36->add_transition('e', qdead);
//
//    q7->add_transition('a', qdead);
//    q7->add_transition('d', qdead);
//    q7->add_transition('o', qdead);
//    q7->add_transition('u', qdead);
//    q7->add_transition('b', q8);
//    q7->add_transition('l', qdead);
//    q7->add_transition('e', qdead);
//
//    q8->add_transition('a', qdead);
//    q8->add_transition('d', qdead);
//    q8->add_transition('o', qdead);
//    q8->add_transition('u', qdead);
//    q8->add_transition('b', qdead);
//    q8->add_transition('l', q9);
//    q8->add_transition('e', qdead);
//
//    q9->add_transition('a', qdead);
//    q9->add_transition('d', qdead);
//    q9->add_transition('o', qdead);
//    q9->add_transition('u', qdead);
//    q9->add_transition('b', qdead);
//    q9->add_transition('l', qdead);
//    q9->add_transition('e', q10);
//
//    q10->add_transition('a', qdead);
//    q10->add_transition('d', qdead);
//    q10->add_transition('o', qdead);
//    q10->add_transition('u', qdead);
//    q10->add_transition('b', qdead);
//    q10->add_transition('l', qdead);
//    q10->add_transition('e', qdead);
//
//    qdead->add_transition('a', qdead);
//    qdead->add_transition('d', qdead);
//    qdead->add_transition('o', qdead);
//    qdead->add_transition('u', qdead);
//    qdead->add_transition('b', qdead);
//    qdead->add_transition('l', qdead);
//    qdead->add_transition('e', qdead);
//
//    std::set<char> alphabet = {'a', 'd', 'o', 'u', 'b', 'l', 'e'};
//    std::set<State *> dfa = {q01411, q2512, q36, q7, q8, q9, q10, qdead};
//    std::vector<std::unordered_map<char, int>> transitionTable = {};
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
//    delete(q01411);
//    delete(q2512);
//    delete(q36);
//    delete(q7);
//    delete(q8);
//    delete(q9);
//    delete(q10);
//    delete(qdead);
//    return 0;
//}