#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <string>

#include "environment.hpp"
#include "lexer.hpp"


class Command {
public:
    virtual ~Command() = default;
    virtual void run(Environment &env) = 0;
    virtual std::string to_string() = 0;
};


class Statement: public Command {
public:
    virtual ~Statement() = default;

    virtual void run(Environment &env) = 0;
    virtual Immediate get_value(Environment &env) = 0;
};

class NoOp: public Statement {
public:
    explicit NoOp(Token token);
    virtual ~NoOp() {
        delete immediate_;
    };

    virtual void run(Environment &env);
    virtual Immediate get_value(Environment &env);

    bool is_immediate() const;
    std::string get_name() const {
        return this->token_.get<std::string>();
    }
    virtual std::string to_string() {
        return get_name();
    }

private:
    const Token token_;
    Immediate *immediate_;
};

// TODO: implement
//class UnaryOp: public Statement {
//public:
//
//private:
//    std::string op_;
//
//};

class BinaryOp: public Statement {
public:
    BinaryOp(std::string &op, Statement *s1, Statement *s2);
    virtual ~BinaryOp() {
        delete s1_;
        delete s2_;
    }

    virtual void run(Environment &env);
    virtual Immediate get_value(Environment &env);
    virtual std::string to_string() {
        std::stringstream ss;
        ss << "(" << op_ << " " << s1_->to_string() << " " << s2_->to_string() << ")";
        return ss.str();
    }

private:
    std::string op_;
    Statement *s1_;
    Statement *s2_;
};


class Block: public Command {
public:
    explicit Block(std::vector<Command *> commands): commands_(std::move(commands)){}

    virtual ~Block() {
        for(auto &cmd: this->commands_) {
            delete cmd;
        }
    }

    virtual void run(Environment &env);
    virtual std::string to_string() {
        std::stringstream ss;
        for(auto &command: commands_) {
            ss << "- " << command->to_string() << std::endl;
        }
        return ss.str();
    }

private:
    std::vector<Command *> commands_;
};

class If: public Command {
public:
    If(Statement *condition, Block *then_clause, Block *else_clause)
    : condition_(condition), then_clause_(then_clause), else_clause_(else_clause) {}
    virtual ~If() {
        delete this->condition_;
        delete this->then_clause_;
        delete this->else_clause_;
    }

    virtual void run(Environment &env);
    virtual std::string to_string() {
        std::stringstream ss;
        ss << "[If (" << condition_->to_string() << ")";
        ss << " (" << std::endl;
        ss << then_clause_->to_string() << ")";
        if (else_clause_ != nullptr) {
            ss << " (" << std::endl;
            ss << else_clause_->to_string() << ")";
        }
        ss << "]";
        return ss.str();
    }

private:
    Statement *condition_;
    Block *then_clause_;
    Block *else_clause_;
};

class While: public Command {
public:
    While(Statement *condition_, Block *block)
    : condition_(condition_), block_(block){}
    virtual ~While() {
        delete this->condition_;
        delete this->block_;
    }

    virtual void run(Environment &env);

    virtual std::string to_string() {
        std::stringstream ss;
        ss << "[While (" << condition_->to_string() << ")";
        ss << " (" << std::endl;
        ss << block_->to_string() << ")";
        ss << "]";
        return ss.str();
    }

private:
    Statement *condition_;
    Block *block_;
};

class With: public Command {
public:
    With(std::string with_type, std::vector<NoOp *> parameters, Block *block)
            : with_type_(std::move(with_type)), parameters_(std::move(parameters)), block_(block){}
    virtual ~With() {
        for (auto param: parameters_) {
            delete param;
        }
        delete this->block_;
    }

    virtual void run(Environment &env);

    virtual std::string to_string() {
        std::stringstream ss;
        ss << "[With " << with_type_ << ":";
        for(auto param: parameters_) {
            ss << " " << param->to_string();
        }
        ss << " (" << std::endl;
        ss << block_->to_string() << ")";
        ss << "]";
        return ss.str();
    }

private:
    std::string with_type_;
    std::vector<NoOp *> parameters_;
    Block *block_;
};

class ShapeDef: public Command {
public:
    ShapeDef(std::string type, const std::vector<NoOp *> &parameters);

    virtual void run(Environment &env);

    void debug_print() const {
        std::cerr << "type_=" << type_ << " ||| ";
        for (auto param: parameters_) {
            std::cerr << param->get_name() << " ||| ";
        }
        std::cerr << "<EOS>" << std::endl;
    }

    virtual std::string to_string() {
        std::stringstream ss;
        ss << "[Shape " << type_;
        for(auto &param: parameters_) {
            ss << " " << param->to_string();
        }
        ss << "]";
        return ss.str();
    }

private:
    const std::string type_;
    const std::vector<NoOp *> parameters_;
};

#endif // COMMANDS_HPP
