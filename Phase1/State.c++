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
    State(int id, bool is_accepting = false, const std::string &token_type = "")
        : id(id), is_accepting(is_accepting), token_type(token_type) {}

    // Add a transition
    void add_transition(char input, State *target)
    {
        transitions[input].insert(target); // Support multiple targets for NFAs
    }

    // Get transitions
    const std::map<char, std::set<State *>> &get_transitions() const
    {
        return transitions;
    }

    // Check if accepting state
    bool is_accepting_state() const { return is_accepting; }

    // Get token type
    const std::string &get_token_type() const { return token_type; }

    // Get ID
    int get_id() const { return id; }

    // Overload the < operator for ordering
    bool operator<(const State &other) const
    {
        return this->id < other.id;
    }

    // For Debugging: Print State Info
    void print_state() const
    {
        std::cout << "State ID: " << id << (is_accepting ? " (Accepting)" : "") << "\n";
        for (const auto &[input, targets] : transitions)
        {
            std::cout << "  On '" << input << "' -> { ";
            for (const auto *target : targets)
            {
                std::cout << target->id << " ";
            }
            std::cout << "}\n";
        }
    }
};
