#ifndef DFACONVERTER_H
#define DFACONVERTER_H

#include <iostream>
#include <utility>
#include <bits/stdc++.h>
#include "State.h"

class NFA
{
public:
    std::set<State> all_states;
    State *start_state;
};

class NFAtoDFAConverter
{
private:
    NFA nfa;
    std::map<std::set<State *>, int> subset_to_state_map;
    std::vector<std::unordered_map<char, int>> dfa_transition_table;
    std::vector<State *> dfa_states;
    State *start_state;

public:
    NFAtoDFAConverter(NFA nfa);

    void create_DFA();

    std::set<State *> e_closure(std::set<State *> T);

    std::vector<std::unordered_map<char, int>> get_dfa_transition_table();

    std::vector<State *> get_dfa_states();

    State *get_start_state();
};

#endif