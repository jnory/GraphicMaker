#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>

#include "../shape/shape.hpp"

std::vector<Shape *> parse(const std::string &shape_str);
Shape *make_shape_from_text(const std::string &line);

#endif // PARSER_HPP
