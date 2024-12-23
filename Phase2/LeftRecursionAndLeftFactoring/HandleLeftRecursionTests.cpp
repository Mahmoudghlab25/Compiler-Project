#include <gtest/gtest.h>
#include <unordered_map>
#include <set>
#include <vector>
#include "HandleLeftRecursion.h"

// Helper function to compare two grammars
bool grammarsAreEqual(const std::unordered_map<std::string, std::vector<std::vector<std::string>>>& g1,
                       const std::unordered_map<std::string, std::vector<std::vector<std::string>>>& g2) {
    if (g1.size() != g2.size()) return false;

    for (const auto& [non_terminal, productions1] : g1) {
        if (g2.find(non_terminal) == g2.end()) return false;

        const auto& productions2 = g2.at(non_terminal);
        if (productions1.size() != productions2.size()) return false;

        for (size_t i = 0; i < productions1.size(); ++i) {
            if (productions1[i] != productions2[i]) return false;
        }
    }

    return true;
}
void print_grammar(std::unordered_map<std::string, std::vector<std::vector<std::string>>> grammar){
    for (const auto& [non_terminal, productions] : grammar) {
        std::cout << non_terminal << " -> ";

        for (size_t i = 0; i < productions.size(); ++i) {
            // Print each production
            for (size_t j = 0; j < productions[i].size(); ++j) {
                std::cout << productions[i][j];
                if (j < productions[i].size() - 1) {
                    std::cout << " ";  // Separate symbols in the production
                }
            }
            if (i < productions.size() - 1) {
                std::cout << " | ";  // Separate productions
            }
        }

        std::cout << std::endl;
    }
}

// Test Case 2: Test indirect left recursion elimination
TEST(HandleLeftRecursionTest, IndirectLeftRecursionElimination) {
    // Grammar with indirect left recursion: A → Bα, B → Aβ
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> grammar = {
            {"A", {{"B", "α"}}},
            {"B", {{"A", "β"}}}
    };

    std::set<std::string> non_terminals = {"A", "B"};
    std::vector<std::string> order_non_terminals = {"A","B"};

    HandleLeftRecursion hlr(grammar, non_terminals,order_non_terminals);
    hlr.eliminate_left_recursion();

    // Expected transformed grammar after eliminating indirect left recursion:
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> expected_grammar = {
            {"A", {{"B","α"}}},
            {"B`", {{"α","β","B`"},{EPSILON}}},
            {"B", {{"B`"}}}
    };

    EXPECT_TRUE(grammarsAreEqual(hlr.getGrammar(), expected_grammar));
}

// Test Case 3: Test indirect left recursion elimination
TEST(HandleLeftRecursionTest, IndirectLeftRecursionElimination2) {
    // Grammar with indirect left recursion: A → Bα, B → Aβ
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> grammar = {
            {"bexpr", {{"bexpr", "or", "bterm"},{"bterm"}}},
            {"bterm", {{"bterm", "and", "bfactor"},{"bfactor"}}},
            {"bfactor",{{"not","bfactor"},{"(","bexpr",")"},{"true"},{"false"}}}
    };

    std::set<std::string> non_terminals = {"bexpr", "bterm","bfactor"};
    std::vector<std::string> order_non_terminals = {"bexpr", "bterm","bfactor"};

    HandleLeftRecursion hlr(grammar, non_terminals, order_non_terminals);
    hlr.eliminate_left_recursion();

    // Expected transformed grammar after eliminating indirect left recursion:
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> expected_grammar = {
            {"bexpr", {{"bterm","bexpr`"}}},
            {"bexpr`", {{"or","bterm","bexpr`"},{EPSILON}}},
            {"bterm", {{"bfactor","bterm`"}}},
            {"bterm`", {{"and", "bfactor","bterm`"},{EPSILON}}},
            {"bfactor",{{"not","bfactor"},{"(","bexpr",")"},{"true"},{"false"}}}
    };

    EXPECT_TRUE(grammarsAreEqual(hlr.getGrammar(), expected_grammar));
}
// Test Case 1: Test direct left recursion elimination
TEST(HandleLeftRecursionTest, DirectLeftRecursionElimination) {
    // Grammar with direct left recursion: A → Aα | β
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> grammar = {
        {"A", {{"A", "α"}, {"β"}}}
    };

    std::set<std::string> non_terminals = {"A"};
    std::vector<std::string> order_non_terminals = {"A"};

    HandleLeftRecursion hlr(grammar, non_terminals,order_non_terminals);
    hlr.eliminate_left_recursion();

    // Expected transformed grammar after eliminating direct left recursion:
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> expected_grammar = {
        {"A", {{"β", "A`"}}},
        {"A`", {{"α", "A`"}, {EPSILON}}}
    };

    EXPECT_TRUE(grammarsAreEqual(hlr.getGrammar(), expected_grammar));
}



// Test Case 4: No left recursion (no changes expected)
TEST(HandleLeftRecursionTest, NoLeftRecursion) {
    // Grammar without recursion: A → β
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> grammar = {
        {"A", {{"β"}}}
    };

    std::set<std::string> non_terminals = {"A"};
    std::vector<std::string> order_non_terminals = {"A"};

    HandleLeftRecursion hlr(grammar, non_terminals, order_non_terminals);
    hlr.eliminate_left_recursion();

    // Expected grammar remains the same as no left recursion is present
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> expected_grammar = {
        {"A", {{"β"}}}
    };

    EXPECT_TRUE(grammarsAreEqual(hlr.getGrammar(), expected_grammar));
}

// Test Case 5: Check if epsilon is handled correctly
TEST(HandleLeftRecursionTest, HandleEpsilon) {
    // Grammar with epsilon: A → Aα | ε
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> grammar = {
        {"A", {{"A", "α"}, {EPSILON}}}
    };

    std::set<std::string> non_terminals = {"A"};
    std::vector<std::string> order_non_terminals = {"A"};

    HandleLeftRecursion hlr(grammar, non_terminals, order_non_terminals);
    hlr.eliminate_left_recursion();

    // Expected transformed grammar after eliminating direct left recursion:
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> expected_grammar = {
        {"A", {{"A`"}}},
        {"A`", {{"α", "A`"}, {EPSILON}}}
    };

    EXPECT_TRUE(grammarsAreEqual(hlr.getGrammar(), expected_grammar));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}