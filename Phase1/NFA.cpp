﻿#include "NFA.h"
#include "State.h"
#include "common.h"

// Constructor
NFA::NFA() : stateCount(0), startState(new State(0, false, "")) {}

// copy constructor
NFA::NFA(NFA &nfa) {
    this->stateCount = nfa.stateCount;
    unordered_map<State *, State *> originalToClone;

    // creating new objects (clone states)
    for (State *originalState: nfa.get_allState()) {
        State *clone = new State(
                originalState->get_id(),
                originalState->is_accepting_state(),
                originalState->get_token_type()
        );
        originalToClone[originalState] = clone;
        this->all_state.push_back(clone);
    }

    // set start state

    this->startState = originalToClone[nfa.getStartState()];

    // set final state
    this->finalState = originalToClone[nfa.getFinalState()];

    // set accepting states
    for (State *originalFinalState: nfa.get_allFinalStates()) {
        this->final_states.push_back(originalToClone[originalFinalState]);
    }

    for (auto &transition: nfa.getTransitions()) {

        // clone transition
        Transition cloneTransition = {
                originalToClone[transition.from],
                originalToClone[transition.to],
                transition.symbol
        };

        // adding corresponding transition to transitions vector
        this->transitions.push_back(cloneTransition);

        // adding transition to inner transitions map of cloneTransition.from
        originalToClone[transition.from]->add_transition(
                cloneTransition.symbol,
                cloneTransition.to
        );
    }
}

// destructor
//NFA::~NFA() {
//    for (State* s : this->all_state) {
//        delete s;
//    }
//}

// Create a basic NFA for a single character
NFA *NFA::basic(char symbol) {
    NFA *nfa = new NFA();
    nfa->stateCount = 2;  // Two states: start and end
    nfa->startState = new State(0, false, "");
    nfa->finalState = new State(1, true, "");
    nfa->add_state(nfa->startState);
    nfa->add_state(nfa->finalState);
    nfa->transitions.push_back({nfa->startState, nfa->finalState, symbol});
    nfa->startState->add_transition(symbol, nfa->finalState);
    return nfa;
}

// Perform union (OR) of two NFAs
NFA *NFA::Union(const NFA &nfa1, const NFA &nfa2) {
    NFA *result = new NFA();
    result->stateCount = nfa1.stateCount + nfa2.stateCount + 2;  // Add new start and final states
    result->startState = new State(0, false, "");
    result->finalState = new State(result->stateCount - 1, true, "");
    result->add_state(result->startState);
    result->add_state(result->finalState);
    int offset = 1 + nfa1.stateCount;        // Offset for the second NFA

    // Add epsilon transitions from the new start state to the old start states
    result->transitions.push_back({result->startState, nfa1.startState, '\0'});
    result->transitions.push_back({result->startState, nfa2.startState, '\0'});
    result->startState->add_transition('\0', nfa1.startState);
    result->startState->add_transition('\0', nfa2.startState);
    for (int i = 0; i < nfa1.all_state.size(); i++) {
        State *s = nfa1.all_state[i];
        s->set_id(s->get_id() + 1);
        result->add_state(s);
    }

    // Copy transitions from the first NFA
    for (const auto &trans: nfa1.transitions) {
        result->transitions.push_back({trans.from, trans.to, trans.symbol});
    }

    for (int i = 0; i < nfa2.all_state.size(); i++) {
        State *s = nfa2.all_state[i];
        s->set_id(s->get_id() + offset);
        result->add_state(s);
    }
    // Copy transitions from the second NFA
    for (const auto &trans: nfa2.transitions) {
        result->transitions.push_back({trans.from, trans.to, trans.symbol});
    }

    // Add epsilon transitions from the old final states to the new final state
    // errorrrrrrrr
    nfa1.finalState->change_acceptance(false);
    nfa2.finalState->change_acceptance(false);
    result->transitions.push_back({nfa1.finalState, result->finalState, '\0'});
    nfa1.finalState->add_transition('\0', result->finalState);
    result->transitions.push_back({nfa2.finalState, result->finalState, '\0'});
    nfa2.finalState->add_transition('\0', result->finalState);
    return result;
}

// Perform concatenation of two NFAs
NFA *NFA::Concatenate(const NFA &nfa1, const NFA &nfa2) {
    NFA *result = new NFA();
    result->stateCount = nfa1.stateCount + nfa2.stateCount;
    result->startState = nfa1.startState;
    result->finalState = nfa2.finalState;
    int offset = nfa1.stateCount;  // Offset for the second NFA
    // Copy transitions from the first NFA
    for (const auto &trans: nfa1.transitions) {
        result->transitions.push_back(trans);
    }
    for (int i = 0; i < nfa1.all_state.size(); i++) {
        State *s = nfa1.all_state[i];
        result->add_state(s);
    }
    nfa1.finalState->change_acceptance(false);
    result->transitions.push_back({nfa1.finalState, nfa2.startState, '\0'});
    nfa1.finalState->add_transition('\0', nfa2.startState);
    for (int i = 0; i < nfa2.all_state.size(); i++) {
        State *s = nfa2.all_state[i];
        s->set_id(s->get_id() + offset);
        result->add_state(s);
    }

    // Copy transitions from the second NFA
    for (const auto &trans: nfa2.transitions) {
        result->transitions.push_back({trans.from, trans.to, trans.symbol});
    }

    return result;
}

// Create the closure (Kleene star) of an NFA
NFA *NFA::Closure(const NFA &nfa) {
    NFA *result = new NFA();
    result->stateCount = nfa.stateCount + 2;  // Add new start and final states
    result->startState = new State(0, false, "");
    result->finalState = new State(result->stateCount - 1, true, "");
    result->add_state(result->startState);
    result->add_state(result->finalState);
    // Add epsilon transitions for the closure
    result->transitions.push_back({result->startState, nfa.startState, '\0'});   // From new start to old start
    result->startState->add_transition('\0', nfa.startState);
    result->transitions.push_back({result->startState, result->finalState, '\0'});    // From new start to new final
    result->startState->add_transition('\0', result->finalState);
    for (int i = 0; i < nfa.all_state.size(); i++) {
        State *s = nfa.all_state[i];
        s->set_id(s->get_id() + 1);
        result->add_state(s);
    }

    // Copy transitions from the original NFA
    for (const auto &trans: nfa.transitions) {
        result->transitions.push_back({trans.from, trans.to, trans.symbol});
    }

    // Add epsilon transitions from old final state to the old start state and new final state
    result->transitions.push_back({nfa.finalState, nfa.startState, '\0'});
    nfa.finalState->add_transition('\0', nfa.startState);
    result->transitions.push_back({nfa.finalState, result->finalState, '\0'});
    nfa.finalState->add_transition('\0', result->finalState);
    nfa.finalState->change_acceptance(false);
    return result;
}

// Create the closure (star) of an NFA
NFA *NFA::positive_closure(const NFA &nfa) {
    NFA *result = new NFA();
    result->stateCount = nfa.stateCount + 2;  // Add new start and final states
    result->startState = new State(0, false, "");
    result->finalState = new State(result->stateCount - 1, true, "");
    result->add_state(result->startState);
    result->add_state(result->finalState);
    // Add epsilon transitions for the closure
    result->transitions.push_back({result->startState, nfa.startState, '\0'});   // From new start to old start
    result->startState->add_transition('\0', nfa.startState);
    for (int i = 0; i < nfa.all_state.size(); i++) {
        State *s = nfa.all_state[i];
        s->set_id(s->get_id() + 1);
        result->add_state(s);
    }

    // Copy transitions from the original NFA
    for (const auto &trans: nfa.transitions) {
        result->transitions.push_back({trans.from, trans.to, trans.symbol});
    }

    // Add epsilon transitions from old final state to the old start state and new final state
    result->transitions.push_back({nfa.finalState, nfa.startState, '\0'});
    nfa.finalState->add_transition('\0', nfa.startState);
    result->transitions.push_back({nfa.finalState, result->finalState, '\0'});
    nfa.finalState->add_transition('\0', result->finalState);
    nfa.finalState->change_acceptance(false);
    return result;
}

NFA *NFA::nfa_sequence(string input, string token) {
    NFA *result = new NFA();
    result = result->basic(input[0]);
    for (int i = 1; i < input.length(); i++) {
        NFA *r2 = new NFA();
        r2 = r2->basic(input[i]);
        result = result->Concatenate(*result, *r2);
    }
    result->finalState->set_token(token);
    return result;
}

NFA *NFA::nfa_options(string input, string token) {
    NFA *result = new NFA();
    result = result->basic(input[0]);
    for (int i = 1; i < input.length(); i++) {
        NFA *r2 = new NFA();
        r2 = r2->basic(input[i]);
        result = result->Union(*result, *r2);
    }
    result->finalState->set_token(token);
    return result;

}

// function to compine all NFA states---------
NFA *NFA::combine(map<string, NFA *> nfas, vector<string> &order) {
    NFA *result = new NFA();
    int offset = 1;
    result->startState = new State(0, false, "");
    result->add_state(result->startState);
    result->stateCount += 1;
    for (int i = 0; i < order.size(); i++) {
        string pattern = order[i];
        NFA *nfa = nfas[order[i]];
        result->transitions.push_back({result->startState, nfa->startState, '\0'});
        result->startState->add_transition('\0', nfa->startState);
        for (int i = 0; i < nfa->all_state.size(); i++) {
            State *s = nfa->all_state[i];
            s->set_id(s->get_id() + offset);
            result->add_state(s);
        }
        // Copy transitions from the original NFA
        for (const auto &trans: nfa->transitions) {
            result->transitions.push_back({trans.from, trans.to, trans.symbol});
        }
        nfa->finalState->set_token(pattern);
        nfa->finalState->change_acceptance(true);
        result->add_final_state(nfa->finalState);
        offset += nfa->stateCount;
        result->stateCount += nfa->stateCount;
    }
    return result;
}

// Display the NFA
void NFA::display() const {
    cout << "States: " << stateCount << endl;
    cout << "Start State: " << startState->get_id() << endl;
    //cout << "Final State: " << finalState->get_id() << endl;
    cout << "Transitions:" << endl;
    for (const auto &trans: transitions) {
        cout << trans.from->get_id() << " --" << (trans.symbol == '\0' ? "epsilon" : string(1, trans.symbol)) << "--> "
             << trans.to->get_id() << endl;
    }

    cout << "States:\n";

    for (const State *s: get_allState()) {
        s->print_state();
    }
}

// Getters
vector<Transition> NFA::getTransitions() const {
    return transitions;
}

State *NFA::getStartState() const {
    return startState;
}

State *NFA::getFinalState() const {
    return finalState;
}

int NFA::getStateCount() const {
    return stateCount;
}