#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "environment.hpp"

class Command {
public:
    virtual void run(Environment &env) = 0;
};

#endif // COMMANDS_HPP
