//
// Created by Mahmo on 11/28/2024.
//

#include "MinimizeDFA.h"

std::set<State *>
MinimizeDFA::minDFA(std::vector<State *> dfa,
                    const std::vector<std::unordered_map<char, int>> &transitionTable,
                    const std::set<char> &alphabet) {
    //1- Start with an initial partition 'p' with two groups, the accepting states group and
    // the non accepting states group in dfa
    std::set<State *> accepting_states;
    std::set<State *> non_accepting_states;
    for (State *state: dfa) {
        if (state->is_accepting_state()) {
            accepting_states.insert(state);
        } else {
            non_accepting_states.insert(state);
        }
    }

    std::vector<std::set<State *>> p = {accepting_states, non_accepting_states};
    std::vector<std::set<State *>> p_new;
    while (true) {
        p_new.clear();
        for (const auto &group: p) {
            //3- foreach group of 'p': partition G into subgroups such that two states s and t are in the same subgroup
            // if and only if for all input symbols a, states s and t have transitions on a to states in the same group
            // of 'p'
            std::map<std::vector<int>, std::set<State *>> subgroups;
            for (State *state: group) {
                std::vector<int> transition_pattern;
                for (char symbol: alphabet) {
                    State *target = *(state->get_transitions().at(symbol)).begin();
                    transition_pattern.push_back(find_group(p, target));
                }
                subgroups[transition_pattern].insert(state);
            }
            //4- replace G in 'p_new' by the set of all subgroups formed;
            for (const auto &subgroup: subgroups) {
                p_new.push_back(subgroup.second);
            }
        }

        //5- If 'p_new' = 'p', let 'p_final' = 'p' and continue with step (6). Otherwise, repeat step (2) with 'p_new'
        // in place of 'p'.
        if (p_new == p) {
            break;
        }
        p = p_new;
    }

    //6- Choose one state in each group of 'p_final' as the representative for that group. The representatives will be
    // the states of the minimum-state DFA (i.e. returned states).
    std::set<State *> minimized_dfa;
    std::map<State *, State *> state_to_representative;

    // Map original states to representatives
    for (const auto &group : p) {
        if (!group.empty()) {
            State *representative = *group.begin();
            minimized_dfa.insert(representative);
            for (State *state : group) {
                state_to_representative[state] = representative;
            }
        }
    }

    // Update transitions for representatives
    for (State *state : minimized_dfa) {
        std::map<char, std::set<State *>> updated_transitions;
        for (char symbol : alphabet) {
            State *original_target = *(state->get_transitions().at(symbol)).begin();
            State *new_target = state_to_representative[original_target];
            updated_transitions[symbol].insert(new_target);
        }
        state->set_transitions(updated_transitions);
    }

    return minimized_dfa;
}
std::map<int, std::unordered_map<char, int>> getReducedTransitionTable(std::set<State *> minDFA){
    std::map<int,std::unordered_map<char, int>> minTransitionTable;
    for (State * current: minDFA) {
        int stateID = current->get_id();
        std::unordered_map<char, int> currTransTable;
        for (const auto &entry : current->get_transitions()) {
            char symbol = entry.first; // The transition symbol (key)
            const State * targetStates = *(entry.second.begin()); // The set of target states (value)
            currTransTable.insert(std::make_pair(symbol,targetStates->get_id()));
        }
        minTransitionTable.insert(std::make_pair(stateID,currTransTable));
    }
    return minTransitionTable;
}
//int main() {
//    //test 1
////    // Step 1: Create States
////    State *A = new State(0, false, "");
////    State *B = new State(1, true, "TOKEN");
////    State *C = new State(2, true, "TOKEN");
////    State *D = new State(3, false, "");
////    State *E = new State(4, true, "TOKEN");
////    State *F = new State(5, true, "TOKEN");
////    State *G = new State(6, true, "TOKEN");
////    State *H = new State(7, true, "TOKEN");
////    State *I = new State(8, false, "dead");
////
////    // Step 2: Define Transitions
////    A->add_transition('0', B);
////    A->add_transition('1', I);
////    B->add_transition('0', D);
////    B->add_transition('1', C);
////    C->add_transition('0', G);
////    C->add_transition('1', D);
////    D->add_transition('0', E);
////    D->add_transition('1', D);
////    E->add_transition('0', I);
////    E->add_transition('1', F);
////    F->add_transition('0', G);
////    F->add_transition('1', D);
////    G->add_transition('0', H);
////    G->add_transition('1', C);
////    H->add_transition('0', E);
////    H->add_transition('1', C);
////    I->add_transition('0', I);
////    I->add_transition('1', I);
////
////
////    // Step 3: Build Transition Table (state IDs)
////    std::vector<std::unordered_map<char, int>> transitionTable = {
////            {{'0', 1}, {'1', 8}}, // A
////            {{'0', 3}, {'1', 2}}, // B
////            {{'0', 6}, {'1', 3}}, // C
////            {{'0', 4}, {'1', 3}}, // D
////            {{'0', 8}, {'1', 5}}, // E
////            {{'0', 6}, {'1', 3}}, // F
////            {{'0', 7}, {'1', 2}}, // G
////            {{'0', 4}, {'1', 2}}, // H
////            {{'0', 8}, {'1', 8}}  // I
////    };
////
////    // Step 4: Define Alphabet
////    std::set<char> alphabet = {'0', '1'};
////
////    // Step 5: Create DFA
////    std::vector<State *> dfa = {A, B, C, D, E, F, G, H, I};
////
////    // Step 6: Minimize DFA
////    MinimizeDFA minimizer;
////    std::set<State *> minimized_dfa = minimizer.minDFA(dfa, transitionTable, alphabet);
////
////    // Step 7: Output Minimized DFA
////    std::cout << "Minimized DFA States:\n";
////    for (State *state: minimized_dfa) {
////        state->print_state();
////    }
////
////    // Clean up allocated memory
////    delete A;
////    delete B;
////    delete C;
////    delete D;
////    delete E;
////    delete F;
////    delete G;
////    delete H;
////    delete I;
//    //test 2
//    // Step 1: Create States
////    State *q0 = new State(0, false, "");
////    State *q1 = new State(1, false, "");
////    State *q2 = new State(2, true, "TOKEN");
////    State *q3 = new State(3, true, "TOKEN");
////
////    // Step 2: Define Transitions
////    q0->add_transition('a', q1);
////    q0->add_transition('b', q2);
////    q1->add_transition('a', q0);
////    q1->add_transition('b', q3);
////    q2->add_transition('a', q3);
////    q2->add_transition('b', q0);
////    q3->add_transition('a', q2);
////    q3->add_transition('b', q1);
////
////    // Step 3: Build Transition Table (state IDs)
////    std::vector<std::unordered_map<char, int>> transitionTable = {
////            {{'a', 1}, {'b', 2}}, // q0
////            {{'a', 0}, {'b', 3}}, // q1
////            {{'a', 3}, {'b', 0}}, // q2
////            {{'a', 2}, {'b', 1}}  // q3
////    };
////
////    // Step 4: Define Alphabet
////    std::set<char> alphabet = {'a', 'b'};
////
////    // Step 5: Create DFA
////    std::vector<State *> dfa = {q0, q1, q2, q3};
////
////    // Step 6: Minimize DFA
////    MinimizeDFA minimizer;
////    std::set<State *> minimized_dfa = minimizer.minDFA(dfa, transitionTable, alphabet);
////
////    // Step 7: Output Minimized DFA
////    std::cout << "Minimized DFA States:\n";
////    for (State *state : minimized_dfa) {
////        state->print_state();
////    }
////
////    // Clean up allocated memory
////    delete q0;
////    delete q1;
////    delete q2;
////    delete q3;
//    //test 3
//    // Step 1: Create States
////    State *A = new State(0, false, "");
////    State *B = new State(1, false, "");
////    State *C = new State(2, false, "");
////    State *D = new State(3, false, "");
////    State *E = new State(4, true, "TOKEN");
////    // Step 2: Define Transitions
////    A->add_transition('a', B);
////    A->add_transition('b', C);
////    B->add_transition('a', B);
////    B->add_transition('b', D);
////    C->add_transition('a', B);
////    C->add_transition('b', C);
////    D->add_transition('a', B);
////    D->add_transition('b', E);
////    E->add_transition('a', B);
////    E->add_transition('b', C);
////
////    // Step 3: Build Transition Table (state IDs)
////    std::vector<std::unordered_map<char, int>> transitionTable = {
////            {{'a', 1}, {'b', 2}}, // A
////            {{'a', 1}, {'b', 3}}, // B
////            {{'a', 1}, {'b', 2}}, // C
////            {{'a', 1}, {'b', 4}}, // D
////            {{'a', 1}, {'b', 2}}  // E
////    };
////
////    // Step 4: Define Alphabet
////    std::set<char> alphabet = {'a', 'b'};
////
////    // Step 5: Create DFA
////    std::vector<State *> dfa = {A, B, C, D, E};
////
////    // Step 6: Minimize DFA
////    MinimizeDFA minimizer;
////    std::set<State *> minimized_dfa = minimizer.minDFA(dfa, transitionTable, alphabet);
////
////    // Step 7: Output Minimized DFA
////    std::cout << "Minimized DFA States:\n";
////    for (State *state : minimized_dfa) {
////        state->print_state();
////    }
////
////    // Clean up allocated memory
////    delete A;
////    delete B;
////    delete C;
////    delete D;
////    delete E;
//    // test4
////    State *C = new State(0, false, "");
////    State *A = new State(1, true, "TOKEN");
////    State *B = new State(2, true, "TOKEN");
////    // Step 2: Define Transitions
////    A->add_transition('0', B);
////    A->add_transition('1', C);
////    B->add_transition('0', C);
////    B->add_transition('1', C);
////    C->add_transition('0', A);
////    C->add_transition('1', C);
////
////    // Step 3: Build Transition Table (state IDs)
////    std::vector<std::unordered_map<char, int>> transitionTable = {
////            {{'0', 1}, {'1', 0}}, //C
////            {{'0', 2}, {'1', 0}}, //A
////            {{'0', 0}, {'1', 0}}  //B
////    };
////
////    // Step 4: Define Alphabet
////    std::set<char> alphabet = {'0', '1'};
////
////    // Step 5: Create DFA
////    std::vector<State *> dfa = {A, B, C};
////
////    // Step 6: Minimize DFA
////    MinimizeDFA minimizer;
////    std::set<State *> minimized_dfa = minimizer.minDFA(dfa, transitionTable, alphabet);
////
////    // Step 7: Output Minimized DFA
////    std::cout << "Minimized DFA States:\n";
////    for (State *state : minimized_dfa) {
////        state->print_state();
////    }
////
////    // Clean up allocated memory
////    delete A;
////    delete B;
////    delete C;
//      //test5
////    State *q1 = new State(0, false, "");
////    State *q2 = new State(1, false, "");
////    State *q4 = new State(2, false, "");
////    State *q5 = new State(3, false, "");
////    State *q3 = new State(4, true, "TOKEN");
////    // Step 2: Define Transitions
////    q1->add_transition('0', q2);
////    q1->add_transition('1', q3);
////    q2->add_transition('0', q1);
////    q2->add_transition('1', q3);
////    q3->add_transition('0', q1);
////    q3->add_transition('1', q4);
////    q4->add_transition('0', q5);
////    q4->add_transition('1', q3);
////    q5->add_transition('0', q5);
////    q5->add_transition('1', q3);
////
////    // Step 3: Build Transition Table (state IDs)
////    std::vector<std::unordered_map<char, int>> transitionTable = {
////            {{'0', 1}, {'1', 4}}, //q1
////            {{'0', 0}, {'1', 4}}, //q2
////            {{'0', 3}, {'1', 4}}, //q4
////            {{'0', 3}, {'1', 4}}, //q5
////            {{'0', 0}, {'1', 2}}  //q3
////    };
////
////    // Step 4: Define Alphabet
////    std::set<char> alphabet = {'0', '1'};
////
////    // Step 5: Create DFA
////    std::vector<State *> dfa = {q1, q2, q3, q4, q5};
////
////    // Step 6: Minimize DFA
////    MinimizeDFA minimizer;
////    std::set<State *> minimized_dfa = minimizer.minDFA(dfa, transitionTable, alphabet);
////
////    // Step 7: Output Minimized DFA
////    std::cout << "Minimized DFA States:\n";
////    for (State *state : minimized_dfa) {
////        state->print_state();
////    }
////
////    // Clean up allocated memory
////    delete q1;
////    delete q2;
////    delete q3;
////    delete q4;
////    delete q5;
//    //test6
////    State *x = new State(1, false, "");
////    State *q2 = new State(2, true, "TOKEN");
////    State *q3 = new State(3, true, "TOKEN");
////    State *q4 = new State(4, true, "TOKEN");
////    State *q6 = new State(6, true, "TOKEN");
////    State *q7 = new State(7, true, "TOKEN");
////    State *q8 = new State(8, false, "dead");
////    // Step 2: Define Transitions
////    x->add_transition('0', q2);
////    x->add_transition('1', q3);
////    q2->add_transition('0', q2);
////    q2->add_transition('1', q4);
////    q3->add_transition('0', x);
////    q3->add_transition('1', q6);
////    q4->add_transition('0', x);
////    q4->add_transition('1', q7);
////    q6->add_transition('0', q8);
////    q6->add_transition('1', x);
////    q7->add_transition('0', q2);
////    q7->add_transition('1', q2);
////    q8->add_transition('0', q8);
////    q8->add_transition('1', q8);
////
////
////    // Step 3: Build Transition Table (state IDs)
////    std::vector<std::unordered_map<char, int>> transitionTable = {
////            {{'0', 1}, {'1', 2}}, //x
////            {{'0', 1}, {'1', 3}}, //q2
////            {{'0', 0}, {'1', 4}}, //q3
////            {{'0', 0}, {'1', 5}}, //q4
////            {{'0', 6}, {'1', 0}}, //q6
////            {{'0', 1}, {'1', 1}}, //q7
////            {{'0', 6}, {'1', 6}}  //q8
////    };
////
////    // Step 4: Define Alphabet
////    std::set<char> alphabet = {'0', '1'};
////
////    // Step 5: Create DFA
////    std::vector<State *> dfa = {x, q2, q3, q4, q6, q7, q8};
////
////    // Step 6: Minimize DFA
////    MinimizeDFA minimizer;
////    std::set<State *> minimized_dfa = minimizer.minDFA(dfa, transitionTable, alphabet);
////
////    // Step 7: Output Minimized DFA
////    std::cout << "Minimized DFA States:\n";
////    for (State *state : minimized_dfa) {
////        state->print_state();
////    }
////
////    // Clean up allocated memory
////    delete x;
////    delete q2;
////    delete q3;
////    delete q4;
////    delete q6;
////    delete q7;
////    delete q8;
//    return 0;
//}