#include <cassert>
#include <unordered_map>
#include <string>

#include "statement.hpp"
#include "commands.hpp"

std::unordered_map<std::string, int> OPERATORS = {
        {"*", 4},
        {"/", 4},
        {"+", 3},
        {"-", 3},
        {"<", 2},
        {">", 2},
        {"<=", 2},
        {">=", 2},
        {"==", 2},
        {"=", 1},
        {"__DUMMY__", 1},
};


struct TokenOrStatement {
    const Token *token;
    Statement *statement;

    explicit TokenOrStatement(const Token *t): token(t), statement(nullptr){}
    explicit TokenOrStatement(Statement *s): token(nullptr), statement(s){}

    Statement *as_statement() const {
        if (this->statement != nullptr) {
            return this->statement;
        } else {
            return new NoOp(*this->token);
        }
    }
};


std::string top_op(std::vector<TokenOrStatement> &stack) {
    auto it = std::end(stack) - 1;
    while (it != std::begin(stack)) {
        auto ts = *it;
        if (ts.token != nullptr) {
            auto surface = ts.token->get<std::string>();
            auto found = OPERATORS.find(surface);
            if (surface == "(") {
                return surface;
            }
            if (found != std::end(OPERATORS)) {
                return surface;
            }
        }
        it--;
    }
    // bottom token must be dummy;
    assert(stack[0].token != nullptr);
    return stack[0].token->get<std::string>();
}

void reduce(std::vector<TokenOrStatement> &stack) {
    assert(stack.size() >= 4);
    auto &s2 = stack.back();
    stack.pop_back();
    auto &op_token = stack.back();
    assert(op_token.token != nullptr);
    stack.pop_back();
    auto &s1 = stack.back();
    stack.pop_back();
    auto op = op_token.token->get<std::string>();
    stack.emplace_back(new BinaryOp(op, s1.as_statement(), s2.as_statement()));
}

Statement *build_statement(TokenIterator &iterator, bool &end_by_closed_paren) {
    // reference: http://fujimura2.fiw-web.net/java/mutter/operator-precedence/index.html
    end_by_closed_paren = false;
    Token dummy("__DUMMY__");
    std::vector<TokenOrStatement> stack;
    stack.emplace_back(&dummy);
    while (!iterator.is_end_of_code()) {
        auto &token = iterator.next(false);
        if (token.is_eos()) {
            break;
        }
        auto surface = token.get<std::string>();
        if (surface == "(") {
            // TODO: check if this is ok.
            stack.emplace_back(&token);
        } else if (surface == ")") {
            auto op = top_op(stack);
            while (op != "(") {
                reduce(stack);
                if (stack.size() < 3) {
                    end_by_closed_paren = true;
                    break;
                }
                op = top_op(stack);
            }
            TokenOrStatement ts = stack.back();
            stack.pop_back(); // ts
            stack.pop_back(); // (
            stack.push_back(ts);
        } else {
            auto found_op = OPERATORS.find(surface);
            auto is_op = found_op != std::end(OPERATORS);
            if (is_op) {
                auto op_rank = found_op->second;
                auto previous_op = top_op(stack);
                if (previous_op == "(") {
                    stack.emplace_back(&token);
                } else {
                    auto found_previous_op = OPERATORS.find(previous_op);
                    auto previous_op_rank = found_previous_op->second;
                    if (op_rank < previous_op_rank) {
                        reduce(stack);
                    } else {
                        stack.emplace_back(&token);
                    }
                }
            } else {
                stack.emplace_back(&token);
            }
        }
    }
    while (stack.size() > 2) {
        reduce(stack);
    }
    return stack[1].as_statement();
}

