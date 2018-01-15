#ifndef TOKENTYPE_H
#define TOKENTYPE_H

#include <iostream>
#include <string>


enum TokenType {
    TokenType_NewLine,
    TokenType_Tab,

    TokenType_Identifier,
    TokenType_Number,
    TokenType_Bool,

    TokenType_LeftParenthesis,
    TokenType_RightParenthesis,

    TokenType_Comma,
    TokenType_Colon,
    TokenType_Semicolon,

    TokenType_Assign,
    TokenType_Plus,
    TokenType_Minus,
    TokenType_Multiply,
    TokenType_Divide,
    TokenType_Greater,
    TokenType_Less,
    TokenType_Equals,
    TokenType_GreaterOrEquals,
    TokenType_LessOrEquals,

    TokenType_KeywordAnd,
    TokenType_KeywordOr,
    TokenType_KeywordDef,
    TokenType_KeywordReturn,
    TokenType_KeywordFor,
    TokenType_KeywordIn,
    TokenType_KeywordRange,
    TokenType_KeywordWhile,
    TokenType_KeywordBreak,
    TokenType_KeywordContinue,

    TokenType_Unknown
};


std::string toString(const TokenType& tokenType);
TokenType getIfKeyword(const std::string& potentialKeyword);
std::ostream& operator<<(std::ostream& os, const TokenType& tokenType);


#endif
