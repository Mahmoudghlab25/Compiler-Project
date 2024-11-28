#include <iostream>
#include <utility>
#include <bits/stdc++.h>
#include "State.h"
#include "DFAConverter.h"
using namespace std;

NFAtoDFAConverter::NFAtoDFAConverter(NFA nfa) : nfa(nfa) {}

void NFAtoDFAConverter::create_DFA()
{
    queue<set<State *>> unprocessed_subsets;
    int id = 0;

    State *nfa_start_state = nfa.start_state;
    set<State *> nfa_start_set;

    nfa_start_set.insert(nfa_start_state);

    set<State *> ss_e_closure = e_closure(nfa_start_set);

    subset_to_state_map[ss_e_closure] = id;
    State *dfa_start_state = new State(id, /*is_accepting=*/false, "");

    this->start_state = dfa_start_state;

    dfa_states.push_back(dfa_start_state);
    unprocessed_subsets.push(ss_e_closure);
    id++;

    while (!unprocessed_subsets.empty())
    {
        set<State *> T = unprocessed_subsets.front();
        unprocessed_subsets.pop();
        int current_dfa_id = subset_to_state_map[T];

        // Process all transitions for the subset
        map<char, set<State *>> combined_transitions;
        for (State *state : T)
        {
            for (const auto &[input, targets] : state->get_transitions())
            {
                combined_transitions[input].insert(targets.begin(), targets.end());
            }
        }

        // Create new transitions in dfa
        for (const auto &[input, target_subset] : combined_transitions)
        {
            set<State *> target_closure = e_closure(target_subset);

            if (subset_to_state_map.find(target_closure) == subset_to_state_map.end())
            {
                subset_to_state_map[target_closure] = id;

                string token_type = "";
                bool is_accepting = std::any_of(
                    target_closure.begin(), target_closure.end(),
                    [&token_type](State *state)
                    {
                        if (state->is_accepting_state())
                        {
                            token_type = state->get_token_type();
                            return true; // Stop once the first accepting state is found
                        }
                        return false;
                    });

                State *target_state = new State(id, is_accepting, token_type);

                dfa_states.push_back(target_state);
                unprocessed_subsets.push(target_closure);
                id++;
            }

            dfa_states[current_dfa_id]->add_transition(input, dfa_states[subset_to_state_map[target_closure]]);
        }

        // Create new transitions in the transition table
        dfa_transition_table.resize(dfa_states.size());
        for (const auto &[input, target_subset] : combined_transitions)
        {
            set<State *> target_closure = e_closure(target_subset);
            int target_id = subset_to_state_map[target_closure];
            dfa_transition_table[current_dfa_id][input] = target_id;
        }
    }
}

set<State *> NFAtoDFAConverter::e_closure(set<State *> T)
{
    set<State *> T_closure;
    stack<State *> st;

    for (State *s : T)
    {
        st.push(s);
        T_closure.insert(s);
    }

    while (!st.empty())
    {
        State *s = st.top();
        st.pop();

        const map<char, set<State *>> &transitions = s->get_transitions();
        auto it = transitions.find('\0');
        if (it != transitions.end())
        {
            const set<State *> &states = it->second;
            for (State *neighbor : states)
            {
                if (T_closure.find(neighbor) == T_closure.end())
                {
                    T_closure.insert(neighbor);
                    st.push(neighbor);
                }
            }
        }
    }

    return T_closure;
}

vector<unordered_map<char, int>> NFAtoDFAConverter::get_dfa_transition_table() { return dfa_transition_table; }

vector<State *> NFAtoDFAConverter::get_dfa_states() { return dfa_states; }

State *NFAtoDFAConverter::get_start_state() { return start_state; }