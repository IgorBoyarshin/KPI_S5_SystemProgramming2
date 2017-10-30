#ifndef TOKENNAME_H
#define TOKENNAME_H

#include <iostream>
#include <string>

enum TokenName {
    TokenName_Identifier,
    TokenName_Keyword,
    TokenName_Operator,
    TokenName_Brackets, // []
    TokenName_Parentheses, // ()
    TokenName_Braces, // {}
    TokenName_Literal,
    TokenName_Semicolon,
    TokenName_Unknown
};

std::ostream& operator<<(std::ostream& os, const TokenName& tokenName);
bool isKeyword(const std::string& lexeme);

#endif
