//
// Created by Maximilian Mittelhammer on 04.12.22.
//

#ifndef LAMBDADB_LEXER_H
#define LAMBDADB_LEXER_H

#include "../code/Repository.h"
#include "Token.h"
#include <map>

namespace sql::lexer {

class Lexer {
private:
    Repository& repository;
    size_t position = 0;
    bool has_next = true;
    std::map<std::string_view, TokenType> keywords;

    /**
     * Check if the given character is a whitespace.
     * @param character The character to check.
     * @return True if the character is a whitespace, false otherwise.
     */
    bool is_whitespace(char c);

    /**
     * Check if the given character is a digit.
     * @param c The character to check.
     * @return True if the character is a digit, false otherwise.
    */
    bool is_digit(char c);

    /**
     * Check if the given character is a valid identifier character.
     * @param c The character to check.
     * @return True if the character is a valid identifier character.
    */
    bool is_identifier(char c);

    /**
     * Get the next character from the code repository.
     * Increase the  pointer.
     * @return The next character.
    */
    char get();

    /**
     * Get the next character from the code repository without advancing the pointer.
     * @return The next character.
    */
    char peek();

public:
    Lexer(Repository& repository);

    /**
     * Get the next token from the code repository.
     * @return The next token.
    */
    Token next();

    /**
     * Get the next token without advancing the pointer.
     * @return The next token.
    */
    Token peek_next();

    /**
     * Check if there is a next token.
     * @return True if there is a next token, false otherwise.
    */
    bool has_next_token();
};
} // namespace sql::lexer

#endif // LAMBDADB_LEXER_H