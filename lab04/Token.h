#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include "TokenName.h"

class Token {
    public:
        Token(const std::string& value);
        Token(const TokenName& name, const std::string& value);

        Token& operator=(const Token& token);

        friend std::ostream& operator<<(std::ostream& os, const Token& token);

    private:
        TokenName m_Name;
        std::string m_Value;
};


std::ostream& operator<<(std::ostream& os, const Token& token);

#endif
