#include "Token.h"


Token::Token(const std::string& value) :
    type(TokenType_Unknown), value(value) {}


Token::Token(const TokenType& type, const std::string& value) :
    type(type), value(value) {}


Token& Token::operator=(const Token& token) {
    this->type = token.type;
    this->value = token.value;

    return *this;
}


std::ostream& operator<<(std::ostream& os, const Token& token) {
    os << token.value << " :: " << token.type;
    return os;
}
