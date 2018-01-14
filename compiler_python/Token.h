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

    public:
        TokenType type;
        std::string value;
};


std::ostream& operator<<(std::ostream& os, const Token& token);

#endif
