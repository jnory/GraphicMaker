#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <cassert>
#include <iostream>
#include <string>
#include <unordered_map>

#include "../io/bitmap.hpp"

enum ImmediateType {
    IM_INTEGER,
    IM_FLOAT,
    // IM_STRING, // TODO: implement
};

union ImmediateValue {
    int64_t integer;
    double floating;
};

class Immediate {
public:
    explicit Immediate(int64_t value): type_(IM_INTEGER), value_({.integer = value}){};
    explicit Immediate(double value): type_(IM_FLOAT), value_({.floating = value}){};

    template<typename T>
    T get() const {
        throw std::runtime_error("unexpected type");
    }

    template<> int get() const {
        assert(type_ == IM_INTEGER);
        // TODO: check value range
        return this->value_.integer;
    }

    template<> int64_t get() const {
        assert(type_ == IM_INTEGER);
        return this->value_.integer;
    }

    template<> double get() const {
        assert(type_ == IM_FLOAT);
        return this->value_.floating;
    }

    template<> size_t get() const {
        assert(type_ == IM_INTEGER);
        // TODO: check value range
        return (size_t) this->value_.integer;
    }

    Immediate operator+(Immediate &other) {
        if (type_ == IM_FLOAT) {
            auto value = value_.floating + other.get_as_float();
            return Immediate(value);
        } else {
            assert(type_ == IM_INTEGER);
            auto value = value_.integer + other.get_as_integer();
            return Immediate(value);
        }
    }

    Immediate operator-(Immediate &other) {
        if (type_ == IM_FLOAT) {
            auto value = value_.floating - other.get_as_float();
            return Immediate(value);
        } else {
            assert(type_ == IM_INTEGER);
            auto value = value_.integer - other.get_as_integer();
            return Immediate(value);
        }
    }

    Immediate operator*(Immediate &other) {
        if (type_ == IM_FLOAT) {
            auto value = value_.floating * other.get_as_float();
            return Immediate(value);
        } else {
            assert(type_ == IM_INTEGER);
            auto value = value_.integer * other.get_as_integer();
            return Immediate(value);
        }
    }

    Immediate operator/(Immediate &other) {
        auto value = get_as_float() / other.get_as_float();
        return Immediate(value);
    }

    Immediate operator>(Immediate &other) {
        bool value;
        if (type_ == IM_FLOAT) {
            value = value_.floating > other.get_as_float();
        } else {
            assert(type_ == IM_INTEGER);
            value = value_.integer > other.get_as_integer();
        }
        return Immediate(value?1ll:0ll);
    }

    Immediate operator<(Immediate &other) {
        bool value;
        if (type_ == IM_FLOAT) {
            value = value_.floating < other.get_as_float();
        } else {
            assert(type_ == IM_INTEGER);
            value = value_.integer < other.get_as_integer();
        }
        return Immediate(value?1ll:0ll);
    }

    Immediate operator==(Immediate &other) {
        bool value;
        if (type_ == IM_FLOAT) {
            value = value_.floating == other.get_as_float();
        } else {
            assert(type_ == IM_INTEGER);
            value = value_.integer == other.get_as_integer();
        }
        return Immediate(value?1ll:0ll);
    }

    Immediate operator<=(Immediate &other) {
        bool value;
        if (type_ == IM_FLOAT) {
            value = value_.floating <= other.get_as_float();
        } else {
            assert(type_ == IM_INTEGER);
            value = value_.integer <= other.get_as_integer();
        }
        return Immediate(value?1ll:0ll);
    }

    Immediate operator>=(Immediate &other) {
        bool value;
        if (type_ == IM_FLOAT) {
            value = value_.floating >= other.get_as_float();
        } else {
            assert(type_ == IM_INTEGER);
            value = value_.integer >= other.get_as_integer();
        }
        return Immediate(value?1ll:0ll);
    }

private:
    double get_as_float() {
        if(type_ == IM_INTEGER) {
            return (double) value_.integer;
        } else {
            assert(type_ == IM_FLOAT);
            return value_.floating;
        }
    }

    int64_t get_as_integer() {
        if(type_ == IM_INTEGER) {
            return value_.integer;
        } else {
            assert(type_ == IM_FLOAT);
            return (int64_t) value_.floating;
        }
    }

    ImmediateType type_;
    ImmediateValue value_;
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

    Immediate set_variable(std::string &name, Immediate &value) {
        auto result = this->variables_.insert({name, value});
        if(!result.second) {
            result.first->second = value;
        }
        return value;
    }

    void debug_print() {
        std::cerr << "===" << std::endl;
        for(auto &p: variables_) {
            std::cerr << p.first << "|||" << p.second.get<int>() << std::endl;
        }
    }

private:
    BitmapFile *file_;
    std::unordered_map<std::string, Immediate> variables_;
};

#endif // ENVIRONMENT_HPP
