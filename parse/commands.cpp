#include <cassert>
#include <string>
#include <utility>

#include "commands.hpp"
#include "shape_def.hpp"

NoOp::NoOp(Token token): token_(std::move(token)){
}

Immediate NoOp::get_value(Environment &env) {
    if (this->token_.is_float()) {
        return Immediate(this->token_.get<double>());
    } else if (this->token_.is_number()) {
        return Immediate(this->token_.get<int64_t>());
    // TODO: implement string
    // } else if (this->token_.is_string()) {
    //    return Immediate(this->token_.get<std::string>());
    } else {
        std::string token_str = this->token_.get<std::string>();
        char first_letter = token_str[0];
        assert(first_letter != '"');
        return env.lookup_variable(token_str);
    }
}

void NoOp::run(Environment &env) {
    this->get_value(env);
}

bool NoOp::is_immediate() const {
    return this->token_.is_number() || this->token_.is_string();
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
        return v1 * v2;
    } else if (this->op_ == "/") {
        return v1 / v2;
    } else if (this->op_ == "+") {
        return v1 + v2;
    } else if (this->op_ == "-") {
        return v1 - v2;
    } else if (this->op_ == "<") {
        return v1 < v2;
    } else if (this->op_ == ">") {
        return v1 > v2;
    } else if (this->op_ == "<=") {
        return v1 <= v2;
    } else if (this->op_ == ">=") {
        return v1 >= v2;
    } else {
        // TODO: implement !=
        assert(this->op_ == "==");
        return v1 == v2;
    }
}

void BinaryOp::run(Environment &env) {
    this->get_value(env);
}


void Block::run(Environment &env) {
    for (auto &cmd: this->commands_) {
        cmd->run(env);
    }
}

void If::run(Environment &env) {
    auto result = this->condition_->get_value(env);
    // else could be null
    if (result.get<int>() == 0 && this->else_clause_ != nullptr) {
        this->else_clause_->run(env);
    } else {
        this->then_clause_->run(env);
    }
}

void While::run(Environment &env) {
    auto result = this->condition_->get_value(env);
    while(result.get<int>() != 0) {
        this->block_->run(env);
        result = this->condition_->get_value(env);
    }
}

ShapeDef::ShapeDef(std::string type, const std::vector<NoOp *> &parameters): type_(std::move(type)), parameters_(parameters) {}

void ShapeDef::run(Environment &env) {
    auto shape = build_shape(this->type_, (Parameters &)this->parameters_, env);
    if (shape == nullptr) {
        throw std::runtime_error("no such type:" + this->type_);
    }
    shape->draw(env.get_file());
    delete shape;
}
