#ifndef STATEMENT_HPP
#define STATEMENT_HPP

#include "commands.hpp"

Statement *build_statement(TokenIterator &iterator, bool &end_by_closed_paren);

#endif // STATEMENT_HPP
