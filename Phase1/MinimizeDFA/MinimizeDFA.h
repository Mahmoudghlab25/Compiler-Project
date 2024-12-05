//
// Created by Mahmo on 11/28/2024.
//
#include "../State.h"
#include <vector>
#include <unordered_map>

#ifndef COMPILER_PROJECT_MINIMIZEDFA_H
#define COMPILER_PROJECT_MINIMIZEDFA_H


class MinimizeDFA {
public:
    std::set<State *> minDFA(std::vector<State *> dfa,
                             const std::vector<std::unordered_map<char, int>> &transitionTable,
                             const std::set<char> &alphabet);

    static std::map<int, std::unordered_map<char, int>> getReducedTransitionTable(std::set<State *> minDFA);

private:
    // Helper function to find the index of the group containing a state
    static int find_group(const std::vector<std::set<State *>> &groups, State *state) {
        for (size_t i = 0; i < groups.size(); ++i) {
            if (groups[i].count(state)) {
                return i;
            }
        }
        return -1; // Not found
    }
};


#endif //COMPILER_PROJECT_MINIMIZEDFA_H
