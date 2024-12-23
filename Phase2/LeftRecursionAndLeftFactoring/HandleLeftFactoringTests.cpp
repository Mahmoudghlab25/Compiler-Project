#include "HandleLeftFactoring.h"
#include <gtest/gtest.h>

using namespace std;

// Test the find_longest_common_prefix method
TEST(HandleLeftFactoringTest, FindLongestCommonPrefix) {
    HandleLeftFactoring hl({},{});

    // Case 1: No productions
    EXPECT_TRUE(hl.find_longest_common_prefix({}).empty());

    // Case 2: Single production
    vector<vector<string>> productions1 = {{"a", "b", "c"}};
    EXPECT_EQ(hl.find_longest_common_prefix(productions1), vector<string>({}));

    // Case 3: Common prefix exists
    vector<vector<string>> productions2 = {
            {"a", "b", "c"},
            {"a", "b", "d"},
            {"a", "b", "e"}
    };
    EXPECT_EQ(hl.find_longest_common_prefix(productions2), vector<string>({"a", "b"}));

    // Case 4: No common prefix
    vector<vector<string>> productions3 = {
            {"x", "y", "z"},
            {"a", "b", "c"}
    };
    EXPECT_TRUE(hl.find_longest_common_prefix(productions3).empty());

    // Case 5: Partial common prefix
    vector<vector<string>> productions4 = {
            {"a", "b", "c"},
            {"a", "b"},
            {"a", "b", "c", "d"}
    };
    EXPECT_EQ(hl.find_longest_common_prefix(productions4), vector<string>({"a", "b"}));
}

// Test the eliminate_left_factoring method
TEST(HandleLeftFactoringTest, EliminateLeftFactoring) {
    unordered_map<string, vector<vector<string>>> grammar = {
            {"A", {{"a", "b", "c"}, {"a", "b", "d"}, {"e"}}}
    };
    HandleLeftFactoring hl(grammar,set<string>({"A"}));

    hl.eliminate_left_factoring();

    auto result = hl.getGrammar();

    // Expected grammar after left factoring
    unordered_map<string, vector<vector<string>>> expected_grammar = {
            {"A", {{"a", "b", "A`1"}, {"e"}}},
            {"A`1", {{"c"}, {"d"}}}
    };

    EXPECT_EQ(result, expected_grammar);
}

// Test eliminate_left_factoring with no changes needed
TEST(HandleLeftFactoringTest, EliminateLeftFactoringNoChanges) {
    unordered_map<string, vector<vector<string>>> grammar = {
            {"B", {{"x", "y"}, {"z"}}}
    };
    HandleLeftFactoring hl(grammar,set<string>({"B"}));

    hl.eliminate_left_factoring();

    auto result = hl.getGrammar();

    // The grammar should remain the same
    EXPECT_EQ(result, grammar);
}

// Test the getGrammar method
TEST(HandleLeftFactoringTest, GetGrammar) {
    unordered_map<string, vector<vector<string>>> grammar = {
            {"C", {{"p", "q"}, {"r", "s"}}}
    };
    HandleLeftFactoring hl(grammar,set<string>({"C"}));

    EXPECT_EQ(hl.getGrammar(), grammar);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
