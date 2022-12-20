#include "Lexer.h"
#include <iostream>

using namespace sql::lexer;

Lexer::Lexer(Repository& repository) : repository(repository) {
    keywords["SELECT"] = TokenType::SELECT;
    keywords["FROM"] = TokenType::FROM;
    keywords["UPDATE"] = TokenType::UPDATE;
    keywords["SET"] = TokenType::SET;
    keywords["INSERT"] = TokenType::INSERT;
    keywords["INTO"] = TokenType::INTO;
    keywords["VALUES"] = TokenType::VALUES;
    keywords["DELETE"] = TokenType::DELETE;
    keywords["WHERE"] = TokenType::WHERE;
    keywords["AND"] = TokenType::AND;
    keywords["OR"] = TokenType::OR;
    keywords["NOT"] = TokenType::NOT;
    keywords["AS"] = TokenType::AS;
    keywords["CREATE"] = TokenType::CREATE;
    keywords["TABLE"] = TokenType::TABLE;
    keywords["DROP"] = TokenType::DROP;
    keywords["ALTER"] = TokenType::ALTER;
    keywords["LEFT"] = TokenType::LEFT;
    keywords["RIGHT"] = TokenType::RIGHT;
    keywords["INNER"] = TokenType::INNER;
    keywords["FULL"] = TokenType::FULL;
    keywords["ORDER"] = TokenType::ORDER;
    keywords["BY"] = TokenType::BY;
    keywords["LIMIT"] = TokenType::LIMIT;
    keywords["ASC"] = TokenType::ASC;
    keywords["DESC"] = TokenType::DESC;
    keywords["JOIN"] = TokenType::JOIN;
    keywords["ON"] = TokenType::ON;
}

bool Lexer::is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

bool Lexer::is_digit(char c) {
    return c >= '0' && c <= '9';
}

bool Lexer::is_identifier(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

char Lexer::get() {
    char c = repository.at(position);
    position++;

    return c;
}

char Lexer::peek() {
    return repository.at(position);
}

Token Lexer::next() {
    if (!has_next || position >= repository.size()) {
        has_next = false;
        return Token(TokenType::END_OF_FILE, {0, 0}, "");
    }

    while (is_whitespace(peek())) {
        get();
    };

    char current = get();
    switch (current) {
        case 0:
            has_next = false;
            return Token(TokenType::END_OF_FILE, {position, 0}, "");
        case '+':
            return Token(
                TokenType::PLUS,
                {position - 1, position},
                repository.get({position - 1, position}));
        case '-':
            return Token(
                TokenType::MINUS,
                {position - 1, position},
                repository.get({position - 1, position}));
        case '*':
            return Token(
                TokenType::ASTERISK,
                {position - 1, position},
                repository.get({position - 1, position}));
        case '/':
            return Token(
                TokenType::DIVIDE,
                {position - 1, position},
                repository.get({position - 1, position}));
        case ',':
            return Token(
                TokenType::COMMA,
                {position - 1, position},
                repository.get({position - 1, position}));
        case '(':
            return Token(
                TokenType::BRACE_OPEN,
                {position - 1, position},
                repository.get({position - 1, position}));
        case ')':
            return Token(
                TokenType::BRACE_CLOSE,
                {position - 1, position},
                repository.get({position - 1, position}));
        case '=':
            return Token(
                TokenType::EQUAL,
                {position - 1, position},
                repository.get({position - 1, position}));
        case '<':
            return Token(
                TokenType::LESS,
                {position - 1, position},
                repository.get({position - 1, position}));
        case '>':
            return Token(
                TokenType::GREATER,
                {position - 1, position},
                repository.get({position - 1, position}));
        case ':':
            if (peek() == '=') {
                get();
                return Token(
                    TokenType::ASSIGN,
                    {position - 2, position},
                    repository.get({position - 2, position}));
            } else {
                repository.logError({position, position}, "Unexpected character");
                return Token(
                    TokenType::UNKNOWN,
                    {position - 1, position},
                    repository.get({position - 1, position}));
            }
        case ';':
            has_next = false;
            return Token(
                TokenType::TERMINATOR,
                {position - 1, position},
                repository.get({position - 1, position}));
        case '\'': {
            size_t start = position - 1;
            size_t length = 1;
            while (peek() != '\'') {
                get();
                length++;
            }
            get();
            length++;
            std::cout << "Found string literal: " << repository.get({start, start + length}) << std::endl;
            return Token(
                TokenType::STRING_LITERAL,
                {start, start + length},
                repository.get({start, start + length}));
        }
        case '"': {
            size_t start = position - 1;
            size_t length = 1;
            bool escape = false;
            while (peek() != '"' || escape) {
                if (escape) {
                    escape = false;
                } else if (peek() == '\\') {
                    escape = true;
                }
                get();
                length++;
            }
            get();
            length++;
            return Token(
                TokenType::STRING_LITERAL,
                {start, start + length},
                repository.get({start, start + length}));
        }
        default:
            if (is_digit(current)) {
                size_t start = position - 1;
                size_t length = 1;
                while (is_digit(peek())) {
                    get();
                    length++;
                }
                return Token(
                    TokenType::INTEGER_LITERAL,
                    {start, start + length},
                    repository.get({start, start + length}));
            } else if (is_identifier(current)) {
                size_t start = position - 1;
                size_t length = 1;
                while (is_identifier(peek())) {
                    get();
                    length++;
                }

                std::string_view value = repository.get({start, start + length});
                TokenType type = keywords.find(value) != keywords.end() ? keywords.at(value) : TokenType::IDENTIFIER;

                return Token{
                    type,
                    {start, start + length},
                    value};
            } else {
                repository.logError({position, position}, "Unexpected character");
                return Token(
                    TokenType::UNKNOWN,
                    {position - 1, position},
                    repository.get({position - 1, position}));
            }
    }
}

Token Lexer::peek_next() {
    Token token = next();
    position = token.position.start;
    return token;
}

bool Lexer::has_next_token() {
    return has_next;
}