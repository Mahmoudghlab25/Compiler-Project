#include <gtest/gtest.h>
#include "FirstAndFollow.h"

class FirstAndFollowTest3 : public ::testing::Test {
protected:
    std::map<std::string, std::vector<std::vector<std::string>>> grammar;
    std::set<std::string> terminals;
    std::set<std::string> non_terminals;
    std::string start_non_terminal;

    void SetUp() override {
        grammar = {
                {"E", {{"T", "E`"}}},
                {"E`", {{"+", "E"}, {EPSILON}}},
                {"T", {{"F", "T`"}}},
                {"T`", {{"T"}, {EPSILON}}},
                {"F", {{"P", "F`"}}},
                {"F`", {{"*", "F"}, {EPSILON}}},
                {"P", {{"(", "E", ")"}, {"a"}, {"b"}, {"Em"}}}
        };
        terminals = {"+", "*", "(", ")", "a", "b", EPSILON, "Em"};
        non_terminals = {"E", "E`", "T", "T`", "F", "F`", "P"};
        start_non_terminal = "E";
    }
};

TEST_F(FirstAndFollowTest3, ComputeNonTerminalFirst) {
    FirstAndFollow ff(grammar, terminals, non_terminals, start_non_terminal);

    std::set<std::string> expected_first_E = {"(", "a", "b", "Em"};
    std::set<std::string> expected_first_T = {"(", "a", "b", "Em"};
    std::set<std::string> expected_first_F = {"(", "a", "b", "Em"};
    std::set<std::string> expected_first_P = {"(", "a", "b", "Em"};

    EXPECT_EQ(ff.compute_non_terminal_first("E"), expected_first_E);
    EXPECT_EQ(ff.compute_non_terminal_first("T"), expected_first_T);
    EXPECT_EQ(ff.compute_non_terminal_first("F"), expected_first_F);
    EXPECT_EQ(ff.compute_non_terminal_first("P"), expected_first_P);
}

TEST_F(FirstAndFollowTest3, CreateFirst) {
    FirstAndFollow ff(grammar, terminals, non_terminals, start_non_terminal);
    ff.create_first();

    std::map<std::string, std::set<std::string>> expected_first = {
            {"E", {"(", "a", "b", "Em"}},
            {"E`", {"+", EPSILON}},
            {"T", {"(", "a", "b", "Em"}},
            {"T`", {"(", "a", "b", "Em", EPSILON}},
            {"F", {"(", "a", "b", "Em"}},
            {"F`", {"*", EPSILON}},
            {"P", {"(", "a", "b", "Em"}}
    };

    EXPECT_EQ(ff.get_first(), expected_first);
}

TEST_F(FirstAndFollowTest3, CreateFollow) {
    FirstAndFollow ff(grammar, terminals, non_terminals, start_non_terminal);
    ff.create_first_and_follow();

    std::map<std::string, std::set<std::string>> expected_follow = {
            {"E", {")", "$"}},
            {"E`", {")", "$"}},
            {"T", {"+", ")", "$"}},
            {"T`", {"+", ")", "$"}},
            {"F", {"(","a","b","Em","+","$",")"}},
            {"F`", {"(","a","b","Em","+","$",")"}},
            {"P", {"(","a","b","Em","+","$",")","*"}}
    };

    EXPECT_EQ(ff.get_follow(), expected_follow);
}

TEST_F(FirstAndFollowTest3, IsTerminal) {
    FirstAndFollow ff(grammar, terminals, non_terminals, start_non_terminal);

    EXPECT_TRUE(ff.is_terminal("+"));
    EXPECT_TRUE(ff.is_terminal("*"));
    EXPECT_TRUE(ff.is_terminal("a"));
    EXPECT_TRUE(ff.is_terminal("b"));
    EXPECT_TRUE(ff.is_terminal("Em"));
    EXPECT_FALSE(ff.is_terminal("E"));
    EXPECT_FALSE(ff.is_terminal("T`"));
}

TEST_F(FirstAndFollowTest3, FullFirstAndFollow) {
    FirstAndFollow ff(grammar, terminals, non_terminals, start_non_terminal);
    ff.create_first_and_follow();

    std::map<std::string, std::set<std::string>> expected_first = {
            {"E", {"(", "a", "b", "Em"}},
            {"E`", {"+", EPSILON}},
            {"T", {"(", "a", "b", "Em"}},
            {"T`", {"(", "a", "b", "Em", EPSILON}},
            {"F", {"(", "a", "b", "Em"}},
            {"F`", {"*", EPSILON}},
            {"P", {"(", "a", "b", "Em"}}
    };

    std::map<std::string, std::set<std::string>> expected_follow = {
            {"E", {")", "$"}},
            {"E`", {")", "$"}},
            {"T", {"+", ")", "$"}},
            {"T`", {"+", ")", "$"}},
            {"F", {"(","a","b","Em","+","$",")"}},
            {"F`", {"(","a","b","Em","+","$",")"}},
            {"P", {"(","a","b","Em","+","$",")","*"}}
    };

    EXPECT_EQ(ff.get_first(), expected_first);
    EXPECT_EQ(ff.get_follow(), expected_follow);
}
