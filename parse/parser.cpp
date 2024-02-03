#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <cassert>

#include "parser.hpp"
#include "tokenizer.hpp"
#include "commands.hpp"
#include "statement.hpp"
#include "shape_def.hpp"

Block *build_block(TokenIterator &iterator, bool &end_by_closed_paren);

If *build_if(TokenIterator &iterator) {
    // <IF> ::= "IF" "(" <STATEMENT> ")" "{" <BLOCK> "}" [ "ELSE" "{" <BLOCK> "}" ]
    assert(!iterator.is_end_of_code());

    auto &paren = iterator.next();
    assert(paren.get<std::string>() == "(");
    bool end_by_closed_paren;
    Statement *condition = build_statement(iterator, end_by_closed_paren);
    assert(end_by_closed_paren);
    auto &open_block = iterator.next();
    assert(open_block.get<std::string>() == "{");
    Block *then_clause = build_block(iterator, end_by_closed_paren);
    assert(end_by_closed_paren);

    auto &next = iterator.look_ahead();
    if(iterator.is_end_of_code() || next.is_eos() || next.get<std::string>() != "ELSE") {
        return new If(condition, then_clause, nullptr);
    }
    iterator.succ();

    auto &open_block2 = iterator.next();
    assert(open_block2.get<std::string>() == "{");
    Block *else_clause = build_block(iterator, end_by_closed_paren);
    assert(end_by_closed_paren);
    return new If(condition, then_clause, else_clause);
}

While *build_while(TokenIterator &iterator) {
    // <WHILE> ::= "WHILE" "(" <STATEMENT> ")" "{" <BLOCK> "}"
    assert(!iterator.is_end_of_code());

    auto &paren = iterator.next();
    assert(paren.get<std::string>() == "(");
    bool end_by_closed_paren;
    Statement *condition = build_statement(iterator, end_by_closed_paren);
    assert(end_by_closed_paren);
    auto &open_block = iterator.next();
    assert(open_block.get<std::string>() == "{");
    Block *loop_clause = build_block(iterator, end_by_closed_paren);
    assert(end_by_closed_paren);
    return new While(condition, loop_clause);
}

ShapeDef *build_shape_def(TokenIterator &iterator) {
    // <SHAPE> ::= <SHAPE_DEF> <EOS>
    // <SHAPE_DEF> ::= <SHAPE_TYPE> <PARAMETERS>
    // <SHAPE_TYPE> ::= "POINT" | "LINE" | "SQUARE" | "TRIANGLE" | "EQUILATERAL_TRIANGLE" | "RECTANGLE" | "REGULAR_POLYGON" | "CIRCLE" | "LOADPNG"
    // <PARAMETERS> ::= <NO_OP>+

    auto &token = iterator.look_ahead();

    if(iterator.is_end_of_code()) {
        return nullptr;
    }

    auto &shape_type = iterator.next();
    if (!is_known_shape(shape_type.get<std::string>())) {
        return nullptr;
    }

    iterator.succ();
    assert(!token.is_eos());
    auto type = token.get<std::string>();

    auto token_ptr = &iterator.next(false);
    std::vector<NoOp *> parameters;
    while(!token_ptr->is_eos()) {
        auto no_op = new NoOp(*token_ptr);
        parameters.push_back(no_op);
    }
    return new ShapeDef(type, parameters);
}

Command *build_command(TokenIterator &iterator) {
    // <COMMAND> ::= <IF> | <WHILE> | <SHAPE> | <STATEMENT>
    auto &token = iterator.next();
    auto str = token.get<std::string>();
    if(str == "IF") {
        return build_if(iterator);
    } else if (str == "WHILE") {
        return build_while(iterator);
    } else {
        auto shape = build_shape_def(iterator);
        if (shape != nullptr) {
            return shape;
        }
        bool end_by_closed_paren;
        Statement *statement = build_statement(iterator, end_by_closed_paren);
        if(end_by_closed_paren) {
            throw std::runtime_error("Parse error: ( not found");
        }
        return statement;
    }
}

Block *build_block(TokenIterator &iterator, bool &end_by_closed_paren) {
    // <BLOCK> ::= <COMMAND>+

    end_by_closed_paren = false;
    std::vector<Command *> commands;
    while(!iterator.is_end_of_code()) {
        auto &token = iterator.look_ahead();
        if (token.get<std::string>() == "}") {
            end_by_closed_paren = true;
            break;
        }
        commands.push_back(build_command(iterator));
    }
    return new Block(commands);
}


Block *parse(const std::string &shape_str)
{
    auto sentences = tokenize(shape_str);
    TokenIterator iterator(std::move(sentences));
    bool end_by_closed_paren;
    auto block = build_block(iterator, end_by_closed_paren);
    assert(!end_by_closed_paren);
    return block;
}