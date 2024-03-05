#include <gtest/gtest.h>
#include "parse/lexer.hpp"
#include "parse/parser.hpp"

TEST(ParserTest, test_parse_statement){
    std::string code = "1 + 2";
    Block *block = parse(code);
    auto actual = block->to_string();
    EXPECT_EQ(actual, "- (+ 1 2)\n");
}

TEST(ParserTest, test_parse_statement_eq){
    std::string code = "a = 1 + 2";
    Block *block = parse(code);
    auto actual = block->to_string();
    EXPECT_EQ(actual, "- (= a (+ 1 2))\n");
}

TEST(ParserTest, test_parse_shape){
    std::string code = "a = 1 + 2\nRECTANGLE 25 50 200 200";
    Block *block = parse(code);
    auto actual = block->to_string();
    EXPECT_EQ(actual, "- (= a (+ 1 2))\n- [Shape RECTANGLE 25 50 200 200]\n");
}

TEST(ParserTest, test_parse_while){
    std::string code = "i = 1\nWHILE(i < 10){\ni = i + 1\n}";
    Block *block = parse(code);
    auto actual = block->to_string();
    EXPECT_EQ(
            actual,
            "- (= i 1)\n"
            "- [While ((< i 10)) (\n"
            "- (= i (+ i 1))\n"
            ")]\n"
    );

}
TEST(ParserTest, test_parse_with_color){
    std::string code = "WITH COLOR(1 2 3){\nPOINT 100 200\n}";
    Block *block = parse(code);
    auto actual = block->to_string();
    EXPECT_EQ(
            actual,
            "- [With COLOR: 1 2 3 (\n"
            "- [Shape POINT 100 200]\n"
            ")]\n"
    );
}

TEST(ParserTest, test_parse_if){
    std::string code =
            "i = 1\n"
            "IF(i < 10){\n"
            "i = i + 1\n"
            "}";
    Block *block = parse(code);
    auto actual = block->to_string();
    EXPECT_EQ(
            actual,
            "- (= i 1)\n"
            "- [If ((< i 10)) (\n"
            "- (= i (+ i 1))\n"
            ")]\n"
    );
}

TEST(ParserTest, test_parse_if_else){
    std::string code =
            "i = 1\n"
            "IF(i < 10){\n"
            "i = i + 1\n"
            "} ELSE {\n"
            "i = i + 2\n"
            "}";

    Block *block = parse(code);
    auto actual = block->to_string();
    EXPECT_EQ(
            actual,
            "- (= i 1)\n"
            "- [If ((< i 10)) (\n"
            "- (= i (+ i 1))\n"
            ") (\n"
            "- (= i (+ i 2))\n"
            ")]\n"
    );
}
