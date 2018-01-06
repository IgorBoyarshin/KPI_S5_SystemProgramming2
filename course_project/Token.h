#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include "TokenType.h"

class Token {
    public:
        Token(const std::string& value);
        Token(const TokenType& type, const std::string& value);

        Token& operator=(const Token& token);

        friend std::ostream& operator<<(std::ostream& os, const Token& token);

        TokenType getType() const;
        std::string getValue() const;

    private:
        TokenType m_Type;
        std::string m_Value;
};


std::ostream& operator<<(std::ostream& os, const Token& token);

#endif
