#include <cassert>
#include <unordered_map>
#include <string>

#include "statement.hpp"
#include "commands.hpp"

std::unordered_map<std::string, std::pair<int, int>> OPERATORS = {
        {"*", {5, 5}},
        {"/", {5, 5}},
        {"+", {4, 4}},
        {"-", {4, 4}},
        {"<", {3, 3}},
        {">", {3, 3}},
        {"<=", {3, 3}},
        {">=", {3, 3}},
        {"==", {3, 3}},
        {"=", {2, 2}},
        {"(", {1, 6}},
        {")", {6, 1}},
        {"__DUMMY__", {0, 0}},
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

std::pair<int, std::string> top_op_rank(std::vector<TokenOrStatement> &stack) {
    auto op = top_op(stack);
    auto found = OPERATORS.find(op);
    auto rank = found->second;
    return {rank.first, op};
}

void pop(std::vector<TokenOrStatement> &stack) {
    assert(stack.size() >= 3);
    auto &s = stack.back();
    stack.pop_back();

    auto &paren_or_dummy = stack.back();
    assert(paren_or_dummy.token != nullptr);
    auto paren_or_dummy_surface = paren_or_dummy.token->get<std::string>();
    assert(paren_or_dummy_surface == "(");
    stack.pop_back();
    stack.push_back(s);
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
    iterator.skip_eos();

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
        auto found_op = OPERATORS.find(surface);
        if (found_op == std::end(OPERATORS)) {
            stack.emplace_back(&token);
            continue;
        }

        auto op_rank = found_op->second.second;
        auto previous_op_rank = top_op_rank(stack);
        bool popped = false;
        while (op_rank <= previous_op_rank.first) {
            if (surface == ")" && previous_op_rank.second == "(") {
                pop(stack);
                popped = true;
                break;
            } else {
                reduce(stack);
            }
            previous_op_rank = top_op_rank(stack);
        }

        auto previous_op = top_op(stack);
        if (surface != ")") {
            stack.emplace_back(&token);
        } else if(previous_op == "__DUMMY__" && !popped) {
            end_by_closed_paren = true;
            break;
        }
    }
    while (stack.size() > 2) {
        reduce(stack);
    }
    return stack[1].as_statement();
}

