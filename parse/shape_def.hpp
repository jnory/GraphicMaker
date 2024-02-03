#ifndef SHAPE_DEF_HPP
#define SHAPE_DEF_HPP

#include <vector>

#include "../shape/shape.hpp"
#include "commands.hpp"

typedef std::vector<NoOp *> Parameters;

bool is_known_shape(const std::string &type);
Shape *build_shape(const std::string &type, Parameters &parameters, Environment &env);

#endif // SHAPE_DEF_HPP
