#ifndef TOKENTYPE_H
#define TOKENTYPE_H

#include <iostream>
#include <string>

enum TokenType {
    TokenType_Identifier,
    TokenType_LiteralBool,
    TokenType_LiteralInt,
    TokenType_LiteralFloat,
    TokenType_Keyword_If,
    TokenType_Keyword_Else,
    TokenType_Keyword_Int,
    TokenType_Keyword_Float,
    TokenType_Keyword_Bool,
    TokenType_Keyword_Void,
    TokenType_Operator,
    TokenType_Operator_Assign,
    TokenType_Brackets_Open, // [
    TokenType_Brackets_Close, // ]
    TokenType_Parentheses_Open, // (
    TokenType_Parentheses_Close, // )
    TokenType_Braces_Open, // {
    TokenType_Braces_Close, // }
    TokenType_Semicolon,
    TokenType_Comma,
    TokenType_Unknown
};


TokenType getTokenTypeFromString(const std::string& str);
std::string toString(TokenType tokenType);

std::ostream& operator<<(std::ostream& os, const TokenType& tokenType);
TokenType getIfKeyword(const std::string& lexeme);

#endif
