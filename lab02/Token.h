#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>

class Token {

    public:
        Token(const std::string& lexeme);

    public:
        const static Token IF;
        const static Token IFCOND;
        const static Token IFBLOCK;
        const static Token ELSE;
        const static Token LEFT_BRACKET;
        const static Token RIGHT_BRACKET;
        const static Token ASSIGN;
        const static Token MUL;
        const static Token SIN;
        const static Token COLUMN;

    private:
        const std::string _Lexeme;

    public:
       friend std::ostream& operator<<(std::ostream& os, Token token);
};

std::ostream& operator<<(std::ostream& os, Token token);

#endif
