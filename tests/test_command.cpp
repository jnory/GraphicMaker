#include <gtest/gtest.h>
#include "parse/lexer.hpp"
#include "parse/parser.hpp"

TEST(CommandTest, test_command_statement_eq){
    Environment env(nullptr);
    std::string code = "a = 1 + 2";
    Block *block = parse(code);
    block->run(env);
    std::string name = "a";
    auto actual = env.lookup_variable(name);
    EXPECT_EQ(actual.get<int>(), 3);
}

TEST(CommandTest, test_command_while){
    Environment env(nullptr);
    std::string code = "i = 1\nWHILE(i < 10){\ni = i + 1\n}";
    Block *block = parse(code);
    block->run(env);
    std::string name = "i";
    auto actual = env.lookup_variable(name);
    EXPECT_EQ(actual.get<int>(), 10);
}

TEST(CommandTest, test_command_if){
    Environment env(nullptr);
    std::string code =
            "i = 1\n"
            "IF(i < 10){\n"
            "i = i + 1\n"
            "}";
    Block *block = parse(code);
    block->run(env);
    std::string name = "i";
    auto actual = env.lookup_variable(name);
    EXPECT_EQ(actual.get<int>(), 2);
}

TEST(CommandTest, test_command_if_else){
    Environment env(nullptr);
    std::string code =
            "i = 1\n"
            "IF(i < 10){\n"
            "i = i + 1\n"
            "} ELSE {\n"
            "i = i + 2\n"
            "}";

    Block *block = parse(code);
    block->run(env);
    std::string name = "i";
    auto actual = env.lookup_variable(name);
    EXPECT_EQ(actual.get<int>(), 2);
}

TEST(CommandTest, test_command_if_else2){
    Environment env(nullptr);
    std::string code =
            "i = 10\n"
            "IF(i < 10){\n"
            "i = i + 1\n"
            "} ELSE {\n"
            "i = i + 2\n"
            "}";

    Block *block = parse(code);
    block->run(env);
    std::string name = "i";
    auto actual = env.lookup_variable(name);
    EXPECT_EQ(actual.get<int>(), 12);
}
