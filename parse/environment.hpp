#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <unordered_map>

#include "../io/bitmap.hpp"

class Immediate {
public:
    explicit Immediate(int value): value_int_(value){};

    template<typename T>
    T get() const {
        throw std::runtime_error("unexpected type");
    }

    template<> int get() const {
        return this->value_int_;
    }

    template<> size_t get() const {
        // TODO: hande properly
        return (size_t)this->value_int_;
    }
    template<> double get() const {
        // TODO: hande properly: data lost
        return (double)this->value_int_;
    }

private:
    int value_int_;
};


class Environment {
public:
    explicit Environment(BitmapFile *file): file_(file){}

    BitmapFile *get_file() {
        return this->file_;
    }

    Immediate lookup_variable(std::string &name) {
        auto found = this->variables_.find(name);
        if (found == std::end(this->variables_)) {
            throw std::runtime_error("variable " + name + " not found");
        }
        return found->second;
    }

    Immediate set_variable(std::string &name, Immediate value) {
        this->variables_.insert({name, value});
        return value;

    }

private:
    BitmapFile *file_;
    std::unordered_map<std::string, Immediate> variables_;
};

#endif // ENVIRONMENT_HPP
