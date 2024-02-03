#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>

#include "../shape/shape.hpp"
#include "tokenizer.hpp"
#include "commands.hpp"

Block *parse(const std::string &shape_str);

#endif // PARSER_HPP
