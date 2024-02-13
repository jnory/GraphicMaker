#include <string>

#include <gtest/gtest.h>
#include "parse/lexer.hpp"

TEST(TokenTest, test_get) {
    Token t1("abc");
    EXPECT_EQ(t1.get<std::string>(), "abc");
    EXPECT_THROW(t1.get<int>(), std::runtime_error);

    Token t2("123");
    EXPECT_EQ(t2.get<std::string>(), "123");
    EXPECT_EQ(t2.get<int>(), 123);

    Token t3("123.4");
    EXPECT_EQ(t3.get<double>(), 123.4);
}

TEST(TokenTest, test_eos) {
    Token t1("abc");
    EXPECT_EQ(t1.is_eos(), false);

    Token t2("abc", false);
    EXPECT_EQ(t2.is_eos(), false);

    Token t3("abc", true);
    EXPECT_EQ(t3.is_eos(), false);

    Token t4("<EOS>", false);
    EXPECT_EQ(t4.is_eos(), false);

    Token t5("<EOS>", true);
    EXPECT_EQ(t5.is_eos(), true);
}

TEST(LexerTest, test_lex_single_line) {
    std::string code = "RECTANGLE 25 50 200 200";
    auto sentences = lex(code);
    ASSERT_EQ(sentences.size(), 1);
    auto &tokens = sentences[0].tokens();
    ASSERT_EQ(tokens.size(), 5);
    EXPECT_EQ(tokens[0].get<std::string>(), "RECTANGLE");
    EXPECT_EQ(tokens[1].get<size_t>(), 25);
    EXPECT_EQ(tokens[2].get<size_t>(), 50);
    EXPECT_EQ(tokens[3].get<size_t>(), 200);
    EXPECT_EQ(tokens[4].get<size_t>(), 200);
}

TEST(LexerTest, test_lex_single_line_with_nl) {
    std::string code = "RECTANGLE 25 50 200 200\n";
    auto sentences = lex(code);
    ASSERT_EQ(sentences.size(), 1);
    auto &tokens = sentences[0].tokens();
    ASSERT_EQ(tokens.size(), 5);
    EXPECT_EQ(tokens[0].get<std::string>(), "RECTANGLE");
    EXPECT_EQ(tokens[1].get<size_t>(), 25);
    EXPECT_EQ(tokens[2].get<size_t>(), 50);
    EXPECT_EQ(tokens[3].get<size_t>(), 200);
    EXPECT_EQ(tokens[4].get<size_t>(), 200);
}

TEST(LexerTest, test_lex_two_lines) {
    std::string code = "RECTANGLE 25 50 200 200\nCIRCLE 100 100 50";
    auto sentences = lex(code);
    ASSERT_EQ(sentences.size(), 2);
    auto &tokens1 = sentences[0].tokens();
    ASSERT_EQ(tokens1.size(), 5);
    auto &tokens2 = sentences[1].tokens();
    ASSERT_EQ(tokens2.size(), 4);
}

TEST(LexerTest, test_lex_two_lines_nl) {
    std::string code = "RECTANGLE 25 50 200 200\nCIRCLE 100 100 50\n";
    auto sentences = lex(code);
    ASSERT_EQ(sentences.size(), 2);
    auto &tokens1 = sentences[0].tokens();
    ASSERT_EQ(tokens1.size(), 5);
    auto &tokens2 = sentences[1].tokens();
    ASSERT_EQ(tokens2.size(), 4);
}

TEST(LexerTest, test_token_iterator_skip_eos) {
    std::string code = "RECTANGLE 25 50 200 200\nCIRCLE 100 100 50\n";
    std::vector<Token> expected = {
            Token("RECTANGLE"),
            Token("25"),
            Token("50"),
            Token("200"),
            Token("200"),
            Token("CIRCLE"),
            Token("100"),
            Token("100"),
            Token("50"),
    };
    TokenIterator it(lex(code));
    size_t i = 0;
    while(!it.is_end_of_code()) {
        auto &token = it.next(true);
        ASSERT_EQ(expected[i].get<std::string>(), token.get<std::string>());
        i++;
    }
}

TEST(LexerTest, test_token_iterator_not_skip_eos) {
    std::string code = "RECTANGLE 25 50 200 200\nCIRCLE 100 100 50\n";
    std::vector<Token> expected = {
            Token("RECTANGLE"),
            Token("25"),
            Token("50"),
            Token("200"),
            Token("200"),
            Token("<EOS>", true),
            Token("CIRCLE"),
            Token("100"),
            Token("100"),
            Token("50"),
            Token("<EOS>", true),
    };
    TokenIterator it(lex(code));
    size_t i = 0;
    while(!it.is_end_of_code()) {
        auto &token = it.next(false);
        ASSERT_EQ(expected[i].get<std::string>(), token.get<std::string>());
        i++;
    }
}
