#include "Token.h"
#include "../code/Repository.h"


Token::Token(TokenType type, Position position, std::string_view value) : type(type), position(position), value(value) {}