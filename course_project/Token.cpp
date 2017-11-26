#include "Token.h"


Token::Token(const std::string& value) :
    m_Type(TokenType_Unknown), m_Value(value) {}


Token::Token(const TokenType& type, const std::string& value) :
    m_Type(type), m_Value(value) {}


TokenType Token::getType() const {
    return m_Type;
}

std::string Token::getValue() const {
    return m_Value;
}

Token& Token::operator=(const Token& token) {
    this->m_Type = token.m_Type;
    this->m_Value = token.m_Value;

    return *this;
}


std::ostream& operator<<(std::ostream& os, const Token& token) {
    os << token.m_Value << " :: " << token.m_Type;
    return os;
}
