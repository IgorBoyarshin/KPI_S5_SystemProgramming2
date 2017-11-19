#include "Token.h"


Token::Token(const std::string& value) :
    m_Name(TokenName_Unknown), m_Value(value) {}


Token::Token(const TokenName& name, const std::string& value) :
    m_Name(name), m_Value(value) {}


TokenName Token::getName() const {
    return m_Name;
}

std::string Token::getValue() const {
    return m_Value;
}

Token& Token::operator=(const Token& token) {
    this->m_Name = token.m_Name;
    this->m_Value = token.m_Value;

    return *this;
}


std::ostream& operator<<(std::ostream& os, const Token& token) {
    os << token.m_Value << " :: " << token.m_Name;

    return os;
}
