#include <gtest/gtest.h>
#include "FirstAndFollow.h"

class FirstAndFollowTest : public ::testing::Test {
protected:
    std::map<std::string, std::vector<std::vector<std::string>>> grammar;
    std::set<std::string> terminals;
    std::set<std::string> non_terminals;
    std::string start_non_terminal;

    void SetUp() override {
        grammar = {
                {"S", {{"A", "B"}}},
                {"A", {{"a"}, {EPSILON}}},
                {"B", {{"b"}}}
        };
        terminals = {"a", "b",EPSILON};
        non_terminals = {"S", "A", "B"};
        start_non_terminal = "S";
    }
};

TEST_F(FirstAndFollowTest, ComputeNonTerminalFirst) {
    FirstAndFollow ff(grammar, terminals, non_terminals, start_non_terminal);
    std::set<std::string> expected_first_A = {"a", EPSILON};
    std::set<std::string> expected_first_B = {"b"};
    std::set<std::string> expected_first_S = {"a", "b"};

    EXPECT_EQ(ff.compute_non_terminal_first("A"), expected_first_A);
    EXPECT_EQ(ff.compute_non_terminal_first("B"), expected_first_B);
    EXPECT_EQ(ff.compute_non_terminal_first("S"), expected_first_S);
}

TEST_F(FirstAndFollowTest, CreateFirst) {
    FirstAndFollow ff(grammar, terminals, non_terminals, start_non_terminal);
    ff.create_first();
    std::map<std::string, std::set<std::string>> expected_first = {
            {"A", {"a", EPSILON}},
            {"B", {"b"}},
            {"S", {"a", "b"}}
    };

    EXPECT_EQ(ff.get_first(), expected_first);
}

TEST_F(FirstAndFollowTest, CreateFollow) {
    FirstAndFollow ff(grammar, terminals, non_terminals, start_non_terminal);
    ff.create_first_and_follow();
    std::map<std::string, std::set<std::string>> expected_follow = {
            {"A", {"b"}},
            {"B", {"$"}},
            {"S", {"$"}}
    };

    EXPECT_EQ(ff.get_follow(), expected_follow);
}

TEST_F(FirstAndFollowTest, IsTerminal) {
    FirstAndFollow ff(grammar, terminals, non_terminals, start_non_terminal);

    EXPECT_TRUE(ff.is_terminal("a"));
    EXPECT_TRUE(ff.is_terminal("b"));
    EXPECT_FALSE(ff.is_terminal("A"));
    EXPECT_FALSE(ff.is_terminal("S"));
}

TEST_F(FirstAndFollowTest, FullFirstAndFollow) {
    FirstAndFollow ff(grammar, terminals, non_terminals, start_non_terminal);
    ff.create_first_and_follow();

    std::map<std::string, std::set<std::string>> expected_first = {
            {"A", {"a", EPSILON}},
            {"B", {"b"}},
            {"S", {"a", "b"}}
    };

    std::map<std::string, std::set<std::string>> expected_follow = {
            {"A", {"b"}},
            {"B", {"$"}},
            {"S", {"$"}}
    };

    EXPECT_EQ(ff.get_first(), expected_first);
    EXPECT_EQ(ff.get_follow(), expected_follow);
}
