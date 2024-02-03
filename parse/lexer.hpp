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
    explicit Sentence(const std::string &line);

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
        std::cerr << "head_sentence_=" << head_sentence_
                  << " ||| token_idx_=" << token_idx_
                  << " ||| sentences_.size()=" << sentences_.size()
                  << std::endl;
    }

    bool is_end_of_code() const {
        auto n = sentences_.size();
        if(head_sentence_ >= n) {
            return true;
        }

        if(head_sentence_ + 1 == n) {
            auto &tokens = sentences_.back().tokens();
            if(token_idx_ >= tokens.size()) {
                return true;
            }
        }

        return false;
    }

private:
    const Token &get_(size_t i, size_t j) const {
        if(sentences_.size() <= i) {
            throw std::runtime_error("index overflow: sentences_ > i");
        }
        auto &tokens = sentences_[i].tokens();
        if(tokens.size() <= j) {
            return TokenIterator::eos_;
        }
        return tokens[j];
    }

    const std::vector<Sentence> sentences_;
    size_t head_sentence_;
    size_t token_idx_;
    size_t head_sentence_last_;
    size_t token_idx_last_;
    static const Token eos_;
};

std::vector<std::string> split_code(const std::string &shape_str);
std::vector<Sentence> lex(const std::string &code);

#endif // LEXER_HPP
