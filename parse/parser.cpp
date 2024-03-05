#include <string>
#include <vector>
#include <iterator>
#include <cassert>

#include "parser.hpp"
#include "lexer.hpp"
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

With *build_with(TokenIterator &iterator) {
    // <WITH> ::= "WITH" <WITH_TYPE> "(" <WITH_PARAMETERS> ")" "{" <BLOCK> "}"
    // <WITH_TYPE> ::= "COLOR"  // TODO: implement AFFINE
    // <WITH_PARAMETERS> ::= <NO_OP>+

    assert(!iterator.is_end_of_code());

    auto &with_type = iterator.next();
    assert(with_type.get<std::string>() == "COLOR");
    auto &paren = iterator.next();
    assert(paren.get<std::string>() == "(");

    auto token_ptr = &iterator.next();
    std::vector<NoOp *> parameters;
    assert(!token_ptr->is_eos());
    while(token_ptr->get<std::string>() != ")") {
        auto no_op = new NoOp(*token_ptr);
        parameters.push_back(no_op);
        token_ptr = &iterator.next();
        assert(!token_ptr->is_eos());
    }

    bool end_by_closed_paren;
    auto &open_block = iterator.next();
    assert(open_block.get<std::string>() == "{");
    Block *with_clause = build_block(iterator, end_by_closed_paren);
    assert(end_by_closed_paren);
    return new With(with_type.get<std::string>(), parameters, with_clause);
}

ShapeDef *build_shape_def(TokenIterator &iterator) {
    // <SHAPE> ::= <SHAPE_DEF> <EOS>
    // <SHAPE_DEF> ::= <SHAPE_TYPE> <PARAMETERS>
    // <SHAPE_TYPE> ::= "POINT" | "LINE" | "SQUARE" | "TRIANGLE" | "EQUILATERAL_TRIANGLE" | "RECTANGLE" | "REGULAR_POLYGON" | "CIRCLE" | "LOADPNG"
    // <PARAMETERS> ::= <NO_OP>+

    if(iterator.is_end_of_code()) {
        return nullptr;
    }

    auto &shape_type = iterator.look_ahead();
    if (!is_known_shape(shape_type.get<std::string>())) {
        return nullptr;
    }

    iterator.succ();
    assert(!shape_type.is_eos());
    auto type = shape_type.get<std::string>();

    auto token_ptr = &iterator.next(false);
    std::vector<NoOp *> parameters;
    while(!token_ptr->is_eos()) {
        auto no_op = new NoOp(*token_ptr);
        parameters.push_back(no_op);
        token_ptr = &iterator.next(false);
    }
    return new ShapeDef(type, parameters);
}

Command *build_command(TokenIterator &iterator) {
    // <COMMAND> ::= <IF> | <WHILE> | <SHAPE> | <WITH> | <STATEMENT>
    auto &token = iterator.look_ahead();
    auto str = token.get<std::string>();
    if(str == "IF") {
        iterator.succ();
        return build_if(iterator);
    } else if (str == "WHILE") {
        iterator.succ();
        return build_while(iterator);
    } else if (str == "WITH") {
        iterator.succ();
        return build_with(iterator);
    } else {
        auto shape = build_shape_def(iterator);
        if (shape != nullptr) {
            // shape->debug_print();
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

    // iterator.debug_print();

    end_by_closed_paren = false;
    std::vector<Command *> commands;
    while(!iterator.is_end_of_code()) {
        // iterator.debug_print();
        auto &token = iterator.look_ahead();
        if (token.get<std::string>() == "}") {
            end_by_closed_paren = true;
            iterator.succ();
            break;
        }
        commands.push_back(build_command(iterator));
    }
    return new Block(commands);
}


Block *parse(const std::string &shape_str)
{
    auto sentences = lex(shape_str);
    //    for(auto &sentence: sentences) {
    //        sentence.debug_print();
    //    }

    TokenIterator iterator(std::move(sentences));
    bool end_by_closed_paren;
    auto block = build_block(iterator, end_by_closed_paren);
    assert(!end_by_closed_paren);
    return block;
}