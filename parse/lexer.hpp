#ifndef LEXER_HPP
#define LEXER_HPP

#include <utility>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>

class Token {
public:
    explicit Token(std::string token, bool is_special_token = false)
        : token_(std::move(token)), is_special_token_(is_special_token) {}

    template<typename T> T get() const {
        T value;
        std::stringstream ss(this->token_);
        ss >> value;
        if (ss.fail()) {
            throw std::runtime_error("failed to parse");
        }
        return value;
    }

    template<> std::string get() const {
        return token_;
    }

    bool is_eos() const {
        return this->is_special_token_ && this->token_ == "<EOS>";
    }

private:
    const std::string token_;
    const bool is_special_token_;
};


using Tokens =  std::vector<Token>;

class Sentence {
public:
    explicit Sentence(size_t line_no, const std::string &line);

    bool empty() const {
        return tokens_.empty();
    }

    const Tokens &tokens() const {
        return this->tokens_;
    }

    void debug_print() const {
        for(auto &token: tokens_) {
            std::cerr << token.get<std::string>() << "|||";
        }
        std::cerr << "<EOS>" << std::endl;
    }

private:
    std::string line_;
    Tokens tokens_;
};


class TokenIterator {
public:
    explicit TokenIterator(const std::vector<Sentence> &&sentences);

    const Token &look_ahead(bool skip_eos = true);
    void succ();
    const Token &next(bool skip_eos = true);

    void debug_print() const {
        std::cerr << "head_=" << head_
                  << " ||| last_=" << last_
                  << " ||| sentences_.size()=" << sentences_.size()
                  << " ||| tokens_.size()=" << tokens_.size()
                  << std::endl;
    }

    bool is_end_of_code() const {
        if(this->tokens_.size() <= this->head_) {
            return true;
        } else if (this->tokens_.size() == this->head_ + 1) {
            return this->tokens_[this->head_].is_eos();
        } else {
            return false;
        }
    }

    void skip_eos() {
        for(; head_ < tokens_.size(); head_++) {
            if(!tokens_[head_].is_eos()){
                break;
            }
        }
        last_ = head_;
    }

private:
    const std::vector<Sentence> sentences_;
    std::vector<Token> tokens_;
    size_t head_;
    size_t last_;
    static const Token eos_;
};

std::vector<std::string> split_code(const std::string &shape_str);
std::vector<Sentence> lex(const std::string &code);

#endif // LEXER_HPP
