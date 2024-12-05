//
// Created by Mahmo on 11/28/2024.
//

#include "MinimizeDFA.h"

//std::set<State *>
//MinimizeDFA::minDFA(std::vector<State *> dfa,
//                    const std::vector<std::unordered_map<char, int>> &transitionTable) {
//    std::set<char> alphabet;
//    for (const auto &table: transitionTable) {
//        for (const auto &pair: table) {
//            alphabet.insert(pair.first);
//        }
//    }
//
//    //1- Start with an initial partition 'p' with two groups, the accepting states group and
//    // the non accepting states group in dfa
//    std::set<State *> accepting_states;
//    std::set<State *> non_accepting_states;
//    for (State *state: dfa) {
//        if (state->is_accepting_state()) {
//            accepting_states.insert(state);
//        } else {
//            non_accepting_states.insert(state);
//        }
//    }
//
//    std::vector<std::set<State *>> p = {accepting_states, non_accepting_states};
//    std::vector<std::set<State *>> p_new;
//    while (true) {
//        p_new.clear();
//        for (const auto &group: p) {
//            //3- foreach group of 'p': partition G into subgroups such that two states s and t are in the same subgroup
//            // if and only if for all input symbols a, states s and t have transitions on a to states in the same group
//            // of 'p'
//            std::map<std::vector<int>, std::set<State *>> subgroups;
//            for (State *state: group) {
//                std::vector<int> transition_pattern;
//                for (char symbol: alphabet) {
//                    std::cout << symbol << std::endl;
//                    State *target = *(state->get_transitions().at(symbol)).begin();
//                    transition_pattern.push_back(find_group(p, target));
//                }
//                subgroups[transition_pattern].insert(state);
//            }
//            //4- replace G in 'p_new' by the set of all subgroups formed;
//            for (const auto &subgroup: subgroups) {
//                p_new.push_back(subgroup.second);
//            }
//        }
//
//        //5- If 'p_new' = 'p', let 'p_final' = 'p' and continue with step (6). Otherwise, repeat step (2) with 'p_new'
//        // in place of 'p'.
//        if (p_new == p) {
//            break;
//        }
//        p = p_new;
//    }
//
//    //6- Choose one state in each group of 'p_final' as the representative for that group. The representatives will be
//    // the states of the minimum-state DFA (i.e. returned states).
//    std::set<State *> minimized_dfa;
//    std::map<State *, State *> state_to_representative;
//
//    // Map original states to representatives
//    for (const auto &group: p) {
//        if (!group.empty()) {
//            State *representative = *group.begin();
//            minimized_dfa.insert(representative);
//            for (State *state: group) {
//                state_to_representative[state] = representative;
//            }
//        }
//    }
//
//    // Update transitions for representatives
//    for (State *state: minimized_dfa) {
//        std::map<char, std::set<State *>> updated_transitions;
//        for (char symbol: alphabet) {
//            State *original_target = *(state->get_transitions().at(symbol)).begin();
//            State *new_target = state_to_representative[original_target];
//            updated_transitions[symbol].insert(new_target);
//        }
//        state->set_transitions(updated_transitions);
//    }
//
//    return minimized_dfa;
//}
std::set<State *> MinimizeDFA::minDFA(
        std::vector<State *> dfa,
        const std::vector<std::unordered_map<char, int>> &transitionTable, const std::set<char> &alphabet) {

    // 1- Create initial partition with multiple groups for accepting states
    // based on the tokens they accept, and one group for non-accepting states.
    std::map<std::string, std::set<State *>> token_to_accepting_states;
    std::set<State *> non_accepting_states;

    for (State *state: dfa) {
        if (state->is_accepting_state()) {
            std::string token = state->get_token_type();
            token_to_accepting_states[token].insert(state);
        } else {
            non_accepting_states.insert(state);
        }
    }

    // Combine all groups into the initial partition
    std::vector<std::set<State *>> p;
    for (const auto &entry: token_to_accepting_states) {
        p.push_back(entry.second); // Group for each token
    }
    p.push_back(non_accepting_states); // Non-accepting states group

    std::vector<std::set<State *>> p_new;
    while (true) {
        p_new.clear();

        for (const auto &group: p) {
            // Partition each group into subgroups based on transition patterns
            std::map<std::vector<int>, std::set<State *>> subgroups;

            for (State *state: group) {
                std::vector<int> transition_pattern;
                for (char symbol: alphabet) {
                    State *target = *(state->get_transitions().at(symbol)).begin();
                    transition_pattern.push_back(find_group(p, target));
                }
                subgroups[transition_pattern].insert(state);
            }

            // Replace the group with its subgroups
            for (const auto &subgroup: subgroups) {
                p_new.push_back(subgroup.second);
            }
        }

        // Check if the partition is stable
        if (p_new == p) {
            break;
        }
        p = p_new;
    }

    // 6- Create the minimized DFA
    std::set<State *> minimized_dfa;
    std::map<State *, State *> state_to_representative;

    for (const auto &group: p) {
        if (!group.empty()) {
            State *representative = *group.begin();
            minimized_dfa.insert(representative);
            for (State *state: group) {
                state_to_representative[state] = representative;
            }
        }
    }

    // Update transitions for representatives
    for (State *state: minimized_dfa) {
        std::map<char, std::set<State *>> updated_transitions;
        for (char symbol: alphabet) {
            State *original_target = *(state->get_transitions().at(symbol)).begin();
            State *new_target = state_to_representative[original_target];
            updated_transitions[symbol].insert(new_target);
        }
        state->set_transitions(updated_transitions);
    }
    return minimized_dfa;
}

std::map<int, std::unordered_map<char, int>> MinimizeDFA::getReducedTransitionTable(std::set<State *> minDFA) {
    std::map<int, std::unordered_map<char, int>> minTransitionTable;
    for (State *current: minDFA) {
        int stateID = current->get_id();
        std::unordered_map<char, int> currTransTable;
        for (const auto &entry: current->get_transitions()) {
            char symbol = entry.first; // The transition symbol (key)
            const State *targetStates = *(entry.second.begin()); // The set of target states (value)
            currTransTable.insert(std::make_pair(symbol, targetStates->get_id()));
        }
        minTransitionTable.insert(std::make_pair(stateID, currTransTable));
    }
    return minTransitionTable;
}
