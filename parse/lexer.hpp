#ifndef LEXER_HPP
#define LEXER_HPP


#include <utility>
#include <sstream>
#include <vector>
#include <string>

class Token {
public:
    explicit Token(std::string token, bool is_special_token = false)
        : token_(std::move(token)), is_special_token_(is_special_token) {}

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


class TokenIterator {
public:
    explicit TokenIterator(const std::vector<Sentence> &&sentences);

    const Token &look_ahead(bool skip_eos = true) const;
    void succ(bool skip_eos=true);
    const Token &next(bool skip_eos = true);

    bool is_end_of_code() const {
        return sentences_.size() <= head_sentence_;
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
    static const Token eos_;
};

std::vector<std::string> split_code(const std::string &shape_str);
std::vector<Sentence> lex(const std::string &code);

#endif // LEXER_HPP
