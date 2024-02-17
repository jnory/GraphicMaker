#include <sstream>

#include "lexer.hpp"

const Token TokenIterator::eos_ = Token("<EOS>", true);
void start_state(size_t line_no, const std::string &line, size_t position, std::vector<Token> &tokens);


std::vector<std::string> split_code(const std::string &shape_str)
{
    std::vector<std::string> lines;
    auto it = std::begin(shape_str);
    auto end = std::end(shape_str);
    while(it != end) {
        auto found = std::find(it, end, '\n');
        std::string line(it, found);
        lines.push_back(line);
        if (found == end) {
            break;
        }
        it = found + 1;
    }

    return lines;
}

enum CharType {
    SPACE,
    OPERATOR,
    PAREN,
    NUMBER,
    ALPHABET,
    DOT,
    OTHER,
};

CharType detect_char_type(char c) {
    switch(c) {
    case ' ':
    case '\t':
        return SPACE;
    case '=':
    case '>':
    case '<':
    case '+':
    case '-':
    case '*':
    case '/':
        return OPERATOR;
    case '(':
    case ')':
    case '{':
    case '}':
        return PAREN;
    case '.':
        return DOT;
    default:
        if ('0' <= c && c <= '9') {
            return NUMBER;
        } else if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z')) {
            return ALPHABET;
        } else {
            return OTHER;
        }
    }
}

std::string build_error_message(size_t line_no, size_t position){
    std::stringstream ss;
    ss << "lexer: invalid character at line " << line_no << ", position " << position << std::endl;
    return ss.str();
}


void goal_state(size_t line_no, const std::string &line, size_t start, size_t end, std::vector<Token> &tokens)
{
    std::string token = line.substr(start, end - start);
    tokens.emplace_back(token);
    return start_state(line_no, line, end, tokens);
}

void alphabet_token_state(size_t line_no, const std::string &line, size_t position, std::vector<Token> &tokens)
{
    for(size_t i = position + 1; i < line.size(); i++) {
        switch(detect_char_type(line[i])) {
        case ALPHABET:
        case NUMBER:
            break;
        case DOT:
        case SPACE:
        case PAREN:
        case OPERATOR:
            return goal_state(line_no, line, position, i, tokens);
        default:
            throw std::runtime_error(build_error_message(line_no, i));
        }
    }
    return goal_state(line_no, line, position, line.size(), tokens);

}

void number_token_state(size_t line_no, const std::string &line, size_t position, std::vector<Token> &tokens)
{
    for(size_t i = position + 1; i < line.size(); i++) {
        switch(detect_char_type(line[i])) {
        case NUMBER:
        case DOT:
            break;
        case SPACE:
        case PAREN:
        case ALPHABET:
        case OPERATOR:
            return goal_state(line_no, line, position, i, tokens);
        default:
            throw std::runtime_error(build_error_message(line_no, i));
        }
    }
    return goal_state(line_no, line, position, line.size(), tokens);
}

void paren_token_state(size_t line_no, const std::string &line, size_t position, std::vector<Token> &tokens)
{
    return goal_state(line_no, line, position, position + 1, tokens);
}

void operator_token_state(size_t line_no, const std::string &line, size_t position, std::vector<Token> &tokens)
{
    for(size_t i = position + 1; i < line.size(); i++) {
        switch(detect_char_type(line[i])) {
        case SPACE:
        case PAREN:
        case NUMBER:
        case ALPHABET:
            return goal_state(line_no, line, position, i, tokens);
        case OPERATOR:
            break;
        case DOT:
        default:
            throw std::runtime_error(build_error_message(line_no, i));
        }
    }
    return goal_state(line_no, line, position, line.size(), tokens);
}

void start_state(size_t line_no, const std::string &line, size_t position, std::vector<Token> &tokens)
{
    for(size_t i = position; i < line.size(); i++) {
        switch(detect_char_type(line[i])) {
        case SPACE:
            break;
        case OPERATOR:
            return operator_token_state(line_no, line, i, tokens);
        case PAREN:
            return paren_token_state(line_no, line, i, tokens);
        case NUMBER:
            return number_token_state(line_no, line, i, tokens);
        case ALPHABET:
            return alphabet_token_state(line_no, line, i, tokens);
        case DOT:
        default:
            throw std::runtime_error(build_error_message(line_no, i));
        }
    }
}

Sentence::Sentence(size_t line_no, const std::string &line): line_(line) {
    start_state(line_no, line, 0, this->tokens_);
}

std::vector<Sentence> lex(const std::string &code){
    auto lines = split_code(code);
    std::vector<Sentence> sentences;
    sentences.reserve(lines.size());
    size_t i = 0;
    for(auto &line: lines) {
        Sentence s(i, line);
        if (!s.empty()) {
            sentences.push_back(s);
        }
        i++;
    }
    return sentences;
}

TokenIterator::TokenIterator(const std::vector<Sentence> &&sentences)
    : sentences_(sentences), head_(0), last_(0)
{
    for(auto &sentence: sentences) {
        auto &tokens = sentence.tokens();
        for(const auto& token: tokens) {
            this->tokens_.push_back(token);
        }
        this->tokens_.push_back(TokenIterator::eos_);
    }
}

const Token &TokenIterator::look_ahead(bool skip_eos) {
    size_t i = this->head_;
    while(skip_eos && this->tokens_[i].is_eos()) {
        i++;
    }
    this->last_ = i;
    return this->tokens_[i];
}

const Token &TokenIterator::next(bool skip_eos) {
    // TODO: improve efficiency
    auto &token = this->look_ahead(skip_eos);
    this->succ();
    return token;
}

void TokenIterator::succ() {
    this->last_++;
    this->head_ = this->last_;
}

