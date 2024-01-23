#include <cassert>
#include <string>

#include "commands.hpp"

std::unordered_map<std::string, int> OPERATORS = {
        {"+", 2},
        {"-", 2},
        {"*", 3},
        {"/", 3},
        {"=", 1},
        {"<", 1},
        {">", 1},
        {"<=", 1},
        {">=", 1},
        {"==", 1},
        {"__DUMMY", 1},
};


struct TokenOrStatement {
    Token *token;
    Statement *statement;

    explicit TokenOrStatement(Token *t): token(t), statement(nullptr){}
    explicit TokenOrStatement(Statement *s): token(nullptr), statement(s){}

    Statement *as_statement() const {
        if (this->statement != nullptr) {
            return this->statement;
        } else {
            return new NoOp(*this->token);
        }
    }
};


NoOp::NoOp(Token &token): token_(token){
}

Immediate NoOp::get_value(Environment &env) {
    std::string token_str = this->token_.get<std::string>();
    char first_letter = token_str[0];
    if ('0' <= first_letter && first_letter <= '9') {
        return {this->token_.get<int>()};
    } else {
        return env.lookup_variable(token_str);
    }
}

void NoOp::run(Environment &env) {
    this->get_value(env);
}

bool NoOp::is_immediate() const {
    std::string token_str = this->token_.get<std::string>();
    char first_letter = token_str[0];
    return '0' <= first_letter && first_letter <= '9';
}


BinaryOp::BinaryOp(std::string &op, Statement *s1, Statement *s2): op_(op), s1_(s1), s2_(s2) {
}

Immediate BinaryOp::get_value(Environment &env) {
    auto v2 = s2_->get_value(env);
    if (this->op_ == "=") {
        NoOp *node =  dynamic_cast<NoOp *>(this->s1_);
        assert(node);
        assert(!node->is_immediate());
        auto name = node->get_name();
        return env.set_variable(name, v2);
    }

    auto v1 = s1_->get_value(env);
    if (this->op_ == "*") {
        return {v1.get<int>() * v2.get<int>()};
    } else if (this->op_ == "/") {
            return {v1.get<int>() / v2.get<int>()};
    } else if (this->op_ == "+") {
        return {v1.get<int>() + v2.get<int>()};
    } else if (this->op_ == "-") {
        return {v1.get<int>() - v2.get<int>()};
    } else if (this->op_ == "<") {
        return {v1.get<int>() < v2.get<int>()};
    } else if (this->op_ == ">") {
        return {v1.get<int>() > v2.get<int>()};
    } else if (this->op_ == "<=") {
        return {v1.get<int>() <= v2.get<int>()};
    } else if (this->op_ == ">=") {
        return {v1.get<int>() >= v2.get<int>()};
    } else {
        assert(this->op_ == "==");
        return {v1.get<int>() == v2.get<int>()};
    }
}

void BinaryOp::run(Environment &env) {
    this->get_value(env);
}

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
    return stack[0].token->get<std::string>();
}

void reduce(std::vector<TokenOrStatement> &stack) {
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

Statement *build_statement(Tokens &tokens) {
    // reference: http://fujimura2.fiw-web.net/java/mutter/operator-precedence/index.html
    Token dummy("__DUMMY__");
    std::vector<TokenOrStatement> stack;
    stack.emplace_back(&dummy);
    for(auto & token : tokens) {
        auto surface = token.get<std::string>();
        if (surface == "(") {
            stack.emplace_back(&token);
        } else if (surface == ")") {
            auto op = top_op(stack);
            while (op != "(") {
                reduce(stack);
                if (stack.size() < 3) {
                    throw std::runtime_error("parse error: ( not found");
                }
                op = top_op(stack);
            }
            TokenOrStatement ts = stack.back();
            stack.pop_back();
            stack.pop_back();
            stack.push_back(ts);
        } else {
            auto found_op = OPERATORS.find(surface);
            auto is_op = found_op != std::end(OPERATORS);
            if (is_op) {
                auto op_rank = *found_op;
                auto previous_op = top_op(stack);
                auto found_previous_op = OPERATORS.find(previous_op);
                auto previous_op_rank = *found_previous_op;
                if (op_rank < previous_op_rank) {
                    reduce(stack);
                } else {
                    stack.emplace_back(&token);
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

void Block::run(Environment &env) {
    for (auto &cmd: this->commands_) {
        cmd->run(env);
    }
}

void If::run(Environment &env) {
    auto result = this->condition_->get_value(env);
    if (result.get<int>() == 0) {
        this->else_clause_.run(env);
    } else {
        this->then_clause_.run(env);
    }
}

void While::run(Environment &env) {
    auto result = this->condition_->get_value(env);
    while(result.get<int>() != 0) {
        this->block_.run(env);
        result = this->condition_->get_value(env);
    }
}
