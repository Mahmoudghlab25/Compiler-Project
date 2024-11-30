#ifndef STATE_H
#define STATE_H

#include <iostream>
#include <map>
#include <set>
#include <string>

class State
{
private:
    int id;                                        // Unique identifier for the state
    bool is_accepting;                             // Whether the state is an accepting state
    std::string token_type;                        // Token type for accepting states
    std::map<char, std::set<State *>> transitions; // Map of transitions

public:
    // Constructor
    State(int id, bool is_accepting, const std::string &token_type);

    // Add a transition
    void add_transition(char input, State *target);

    // Get transitions
    const std::map<char, std::set<State *>> &get_transitions() const;

    // Check if accepting state
    bool is_accepting_state() const;

    // Get token type
    const std::string &get_token_type() const;

    // set token type
    void set_token(string token);

    // Get ID
    int get_id() const;

    // Set ID
    void set_id(int newId);
    // change acceptance
    void change_acceptance(bool newAcc);

    // Overload the < operator for ordering
    bool operator<(const State &other) const;

    // For Debugging: Print State Info
    void print_state() const;
};

#endif