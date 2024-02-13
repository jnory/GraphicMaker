#include <gtest/gtest.h>
#include "parse/lexer.hpp"
#include "parse/statement.hpp"

TEST(StatementTest, test_build_statement_add){
    std::string code = "1 + 2";
    TokenIterator it(lex(code));
    bool end_by_closed_paren;
    auto statement = build_statement(it, end_by_closed_paren);
    auto actual = statement->to_string();
    EXPECT_EQ(actual, "(+ 1 2)");
}

TEST(StatementTest, test_build_statement_eq){
    std::string code = "a = 3";
    TokenIterator it(lex(code));
    bool end_by_closed_paren;
    auto statement = build_statement(it, end_by_closed_paren);
    auto actual = statement->to_string();
    EXPECT_EQ(actual, "(= a 3)");
}

TEST(StatementTest, test_build_statement_prod_add){
    std::string code = "1 + 2 * 3";
    TokenIterator it(lex(code));
    bool end_by_closed_paren;
    auto statement = build_statement(it, end_by_closed_paren);
    auto actual = statement->to_string();
    EXPECT_EQ(actual, "(+ 1 (* 2 3))");
}

TEST(StatementTest, test_build_statement_eq_add){
    std::string code = "a = 1 + 2";
    TokenIterator it(lex(code));
    bool end_by_closed_paren;
    auto statement = build_statement(it, end_by_closed_paren);
    auto actual = statement->to_string();
    EXPECT_EQ(actual, "(= a (+ 1 2))");
}
TEST(StatementTest, test_build_statement_eq_lt){
    std::string code = "a = 1 < 2";
    TokenIterator it(lex(code));
    bool end_by_closed_paren;
    auto statement = build_statement(it, end_by_closed_paren);
    auto actual = statement->to_string();
    EXPECT_EQ(actual, "(= a (< 1 2))");
}

TEST(StatementTest, test_build_statement_eq_lt_add){
    std::string code = "a = 1 < 2 + 3";
    TokenIterator it(lex(code));
    bool end_by_closed_paren;
    auto statement = build_statement(it, end_by_closed_paren);
    auto actual = statement->to_string();
    EXPECT_EQ(actual, "(= a (< 1 (+ 2 3)))");
}

TEST(StatementTest, test_build_statement_prod_add_par){
    std::string code = "(1 + 2) * 3";
    TokenIterator it(lex(code));
    bool end_by_closed_paren;
    auto statement = build_statement(it, end_by_closed_paren);
    auto actual = statement->to_string();
    EXPECT_EQ(actual, "(* (+ 1 2) 3)");
}

TEST(StatementTest, test_build_statement_prod_add_par2){
    std::string code = "3 * (1 + 2)";
    TokenIterator it(lex(code));
    bool end_by_closed_paren;
    auto statement = build_statement(it, end_by_closed_paren);
    auto actual = statement->to_string();
    EXPECT_EQ(actual, "(* 3 (+ 1 2))");
    EXPECT_FALSE(end_by_closed_paren);
}

TEST(StatementTest, test_build_statement_multiple_parens){
    std::string code = "(1 + 2) * ((3 + 4) / 5)";
    TokenIterator it(lex(code));
    bool end_by_closed_paren;
    auto statement = build_statement(it, end_by_closed_paren);
    auto actual = statement->to_string();
    EXPECT_EQ(actual, "(* (+ 1 2) (/ (+ 3 4) 5))");
    EXPECT_FALSE(end_by_closed_paren);
}
