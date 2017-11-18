#ifndef TOKENNAME_H
#define TOKENNAME_H

#include <iostream>
#include <string>

enum TokenName {
    TokenName_Identifier,
    TokenName_Literal,
    TokenName_Keyword_If,
    TokenName_Keyword_Else,
    TokenName_Operator,
    TokenName_Operator_Equals,
    TokenName_Brackets_Open, // [
    TokenName_Brackets_Close, // ]
    TokenName_Parentheses_Open, // (
    TokenName_Parentheses_Close, // )
    TokenName_Braces_Open, // {
    TokenName_Braces_Close, // }
    TokenName_Semicolon,
    TokenName_Comma,
    TokenName_Unknown
};

std::ostream& operator<<(std::ostream& os, const TokenName& tokenName);
bool isKeyword(const std::string& lexeme);

#endif
