#include <iostream>
#include <map>
#include <set>
#include <string>
#include "State.h"

State::State(int id, bool is_accepting, const std::string &token_type)
        : id(id), is_accepting(is_accepting), token_type(token_type) {}

// Add a transition
void State::add_transition(char input, State *target) {
    transitions[input].insert(target); // Support multiple targets for NFAs
}

// Get transitions
const std::map<char, std::set<State *>> &State::get_transitions() const {
    return transitions;
}

// Check if accepting state
bool State::is_accepting_state() const { return is_accepting; }
// Check if dead state
bool State::is_dead_state() const {
    if (this->is_accepting_state()) {
        return false;
    }
    std::string tokenType = this->get_token_type();
    if(tokenType.empty() || !(tokenType == "dead")){
        return false;
    }
    return true;
}

// Get token type
const std::string &State::get_token_type() const { return token_type; }
// Set token type
void State::set_token(std::string token) { token_type = token; }
// Get ID
int State::get_id() const { return id; }
//Set id
void State::set_id(int newId) { id = newId; }
// change acceptance
void State::change_acceptance(bool newAcc) {
    is_accepting = newAcc;
}
// Overload the < operator for ordering
bool State::operator<(const State &other) const {
    return this->id < other.id;
}

// For Debugging: Print State Info
void State::print_state() const {
    std::cout << "State ID: " << id << (is_accepting ? " (Accepting)" : "") << "\n";
    for (const auto &[input, targets]: transitions) {
        std::cout << "  On '" << input << "' -> { ";
        for (const auto *target: targets) {
            std::cout << target->id << " ";
        }
        std::cout << "}\n";
    }
}
