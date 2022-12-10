#include "Token.h"
#include "../code/Repository.h"


sql::lexer::Token::Token(TokenType type, Position position, std::string_view value) : type(type), position(position), value(value) {}