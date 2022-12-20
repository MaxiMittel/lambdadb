//
// Created by Maximilian Mittelhammer on 04.12.22.
//

#ifndef LAMBDADB_TOKEN_H
#define LAMBDADB_TOKEN_H

#include <cstdlib>
#include <iostream>
#include <string_view>
#include "../code/Repository.h"

namespace sql::lexer {

enum TokenType {
    // Other
    UNKNOWN,
    END_OF_FILE,

    // Identifiers
    IDENTIFIER,

    // Literals
    INTEGER_LITERAL,
    FLOAT_LITERAL,
    STRING_LITERAL,

    // Keywords
    SELECT,
    FROM,
    UPDATE,
    SET,
    INSERT,
    INTO,
    VALUES,
    DELETE,
    WHERE,
    AND,
    OR,
    NOT,
    AS,
    CREATE,
    TABLE,
    DROP,
    ALTER,
    LEFT,
    RIGHT,
    INNER,
    FULL,
    ORDER,
    BY,
    LIMIT,
    ASC,
    DESC,
    JOIN,
    ON,

    // Symbols
    TERMINATOR,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    COMMA,
    SEMICOLON,
    EQUAL,
    NOT_EQUAL,
    LESS,
    LESS_EQUAL,
    GREATER,
    GREATER_EQUAL,
    ASSIGN,
    DOT,
    ASTERISK,
    BACKTICK,
    SINGLE_QUOTE,
    DOUBLE_QUOTE,
    PARENTHESIS_OPEN,
    PARENTHESIS_CLOSE,
    BRACE_OPEN,
    BRACE_CLOSE,

    INTEGER,
    VARCHAR
};

class Token {
    public:
    TokenType type;
    Position position;
    std::string_view value;

    Token(TokenType type, Position position, std::string_view value);

    friend std::ostream& operator<<(std::ostream& os, const Token& token){
        os << "Token{type=" << token.type << ", position=" << token.position.start << ":" << token.position.end << ", value=" << token.value << "}";
        return os;
    }
};

}

#endif // LAMBDADB_TOKEN_H