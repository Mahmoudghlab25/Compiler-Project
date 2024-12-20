#include <gtest/gtest.h>
#include "FirstAndFollow.h"

class FirstAndFollowTest2 : public ::testing::Test {
protected:
    std::map<std::string, std::vector<std::vector<std::string>>> grammar;
    std::set<std::string> terminals;
    std::set<std::string> non_terminals;
    std::string start_non_terminal;

    void SetUp() override {
        grammar = {
                {"E", {{"T", "E`"}}},
                {"E`", {{"+", "T", "E`"}, {"\0"}}},
                {"T", {{"F", "T`"}}},
                {"T`", {{"*", "F", "T`"}, {"\0"}}},
                {"F", {{"(", "E", ")"}, {"id"}}}
        };
        terminals = {"+", "*", "(", ")", "id", "\0"};
        non_terminals = {"E", "E`", "T", "T`", "F"};
        start_non_terminal = "E";
    }
};

TEST_F(FirstAndFollowTest2, ComputeNonTerminalFirst) {
    FirstAndFollow ff(grammar, terminals, non_terminals, start_non_terminal);
    std::set<std::string> expected_first_E = {"(", "id"};
    std::set<std::string> expected_first_T = {"(", "id"};
    std::set<std::string> expected_first_F = {"(", "id"};

    EXPECT_EQ(ff.compute_non_terminal_first("E"), expected_first_E);
    EXPECT_EQ(ff.compute_non_terminal_first("T"), expected_first_T);
    EXPECT_EQ(ff.compute_non_terminal_first("F"), expected_first_F);
}

TEST_F(FirstAndFollowTest2, CreateFirst) {
    FirstAndFollow ff(grammar, terminals, non_terminals, start_non_terminal);
    ff.create_first();
    std::map<std::string, std::set<std::string>> expected_first = {
            {"E", {"(", "id"}},
            {"E`", {"+", "\0"}},
            {"T", {"(", "id"}},
            {"T`", {"*", "\0"}},
            {"F", {"(", "id"}}
    };

    EXPECT_EQ(ff.get_first(), expected_first);
}

TEST_F(FirstAndFollowTest2, CreateFollow) {
    FirstAndFollow ff(grammar, terminals, non_terminals, start_non_terminal);
    ff.create_first_and_follow();
    std::map<std::string, std::set<std::string>> expected_follow = {
            {"E", {")", "$"}},
            {"E`", {")", "$"}},
            {"T", {"+", ")", "$"}},
            {"T`", {"+", ")", "$"}},
            {"F", {"*", "+", ")", "$"}}
    };

    EXPECT_EQ(ff.get_follow(), expected_follow);
}

TEST_F(FirstAndFollowTest2, IsTerminal) {
    FirstAndFollow ff(grammar, terminals, non_terminals, start_non_terminal);

    EXPECT_TRUE(ff.is_terminal("+"));
    EXPECT_TRUE(ff.is_terminal("*"));
    EXPECT_TRUE(ff.is_terminal("id"));
    EXPECT_FALSE(ff.is_terminal("E"));
    EXPECT_FALSE(ff.is_terminal("T`"));
}

TEST_F(FirstAndFollowTest2, FullFirstAndFollow) {
    FirstAndFollow ff(grammar, terminals, non_terminals, start_non_terminal);
    ff.create_first_and_follow();

    std::map<std::string, std::set<std::string>> expected_first = {
            {"E", {"(", "id"}},
            {"E`", {"+", "\0"}},
            {"T", {"(", "id"}},
            {"T`", {"*", "\0"}},
            {"F", {"(", "id"}}
    };

    std::map<std::string, std::set<std::string>> expected_follow = {
            {"E", {")", "$"}},
            {"E`", {")", "$"}},
            {"T", {"+", ")", "$"}},
            {"T`", {"+", ")", "$"}},
            {"F", {"*", "+", ")", "$"}}
    };

    EXPECT_EQ(ff.get_first(), expected_first);
    EXPECT_EQ(ff.get_follow(), expected_follow);
}
