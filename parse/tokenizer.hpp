#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP


#include <utility>
#include <sstream>
#include <vector>
#include <string>

class Token {
public:
    explicit Token(std::string token): token_(std::move(token)) {}

    template<typename T> T get() const {
        T value;
        std::stringstream  ss(this->token_);
        ss >> value;
        if (ss.fail()) {
            throw std::runtime_error("failed to parse");
        }
        return value;
    }

    template<> std::string get() const {
        return token_;
    }

private:
    std::string token_;
};


using Tokens =  std::vector<Token>;

class Sentence {
public:
    explicit Sentence(const std::string &line);

    const std::string &source() const {
        return this->line_;
    }

    bool empty() const {
        return tokens_.empty();
    }

    const Tokens &tokens() const {
        return this->tokens_;
    }

private:
    std::string line_;
    Tokens tokens_;
};

std::vector<std::string> split_code(const std::string &shape_str);
std::vector<Sentence> tokenize(const std::string &code);

#endif // TOKENIZER_HPP
