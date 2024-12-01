#ifndef NFA_H
#define NFA_H

#include <iostream>
#include <vector>
#include <set>
#include "State.h"

using namespace std;

// Structure representing a transition
struct Transition {
    State* from;           // Start state
    State* to;             // End state
    char symbol;        // Transition symbol ('\0' represents epsilon)
};

// Class representing an NFA
class NFA {
private:
    int stateCount;                          // Total number of states
    vector<Transition> transitions;          // List of transitions
    State* finalState;                    // final state
    State* startState;                          // Start state
    vector <State*> all_state;
    vector <State*> final_states;
public:
    // Constructor
    NFA();

    // Static method to create a basic NFA for a single character
    NFA basic(char symbol);

    // Static method to perform union of two NFAs
    NFA Union(const NFA& nfa1, const NFA& nfa2);

    // Static method to perform concatenation of two NFAs
    NFA Concatenate(const NFA& nfa1, const NFA& nfa2);

    // Static method to create the closure (Kleene star) of an NFA
    NFA Closure(const NFA& nfa);

    // Static method to create the closure (positive) of an NFA
    NFA positive_closure(const NFA& nfa);

    NFA nfa_sequence(string input, string token_type);
    NFA nfa_options(string input, string token_type);
    // add states
    void add_state(State* state){all_state.push_back(state);}
    // get all states
    vector <State*> get_allState(){return all_state;}
    // add final states
    void add_final_state(State* state){final_states.push_back(state);}
    // get all final states
    vector <State*> get_allFinalStates(){return final_states;}
    // Compine all states
    NFA compine(map<string,NFA> nfas);
    // Display the NFA
    void display() const;

    // Getters for transitions, start state, final states, and state count
    vector<Transition> getTransitions() const;
    State getStartState() const;
    State getFinalStates() const;
    int getStateCount() const;
};

#endif 
