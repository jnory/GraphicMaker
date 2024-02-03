#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "environment.hpp"
#include "tokenizer.hpp"


class Command {
public:
    virtual ~Command() = default;
    virtual void run(Environment &env) = 0;
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
    virtual ~NoOp() = default;

    virtual void run(Environment &env);
    virtual Immediate get_value(Environment &env);

    bool is_immediate() const;
    std::string get_name() const {
        return this->token_.get<std::string>();
    }

private:
    const Token token_;
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

private:
    std::string op_;
    Statement *s1_;
    Statement *s2_;
};


class Block: public Command {
public:
    explicit Block(std::vector<Command *> &commands): commands_(commands){}

    virtual ~Block() {
        for(auto &cmd: this->commands_) {
            delete cmd;
        }
    }

    virtual void run(Environment &env);

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

private:
    Statement *condition_;
    Block *block_;
};

class ShapeDef: public Command {
public:
    ShapeDef(std::string type, const std::vector<NoOp *> &parameters);

    virtual void run(Environment &env);

private:
    const std::string type_;
    const std::vector<NoOp *> parameters_;
};

#endif // COMMANDS_HPP
