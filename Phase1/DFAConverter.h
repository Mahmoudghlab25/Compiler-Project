#ifndef DFACONVERTER_H
#define DFACONVERTER_H

#include <iostream>
#include <utility>
#include <bits/stdc++.h>
#include "State.h"
#include "NFA.h"

class NFAtoDFAConverter
{
private:
    NFA nfa;
    std::map<std::set<State *>, int> subset_to_state_map;
    std::vector<std::unordered_map<char, int>> dfa_transition_table;
    std::vector<State *> dfa_states;
    State *start_state;
    set<char> inputs;

public:
    NFAtoDFAConverter(NFA nfa, set<char> inputs);

    void create_DFA();

    std::set<State *> e_closure(std::set<State *> T);

    bool is_valid_transition(char input);

    std::vector<std::unordered_map<char, int>> get_dfa_transition_table();

    std::vector<State *> get_dfa_states();

    State *get_start_state();
};

#endif