#include "TokenType.h"


// TokenType getTokenTypeFromString(const std::string& str) {
// }


std::string toString(TokenType tokenType) {
    switch(tokenType) {
        case TokenType_Identifier:
            return "Identifier";
        case TokenType_LiteralBool:
            return "Literal bool";
        case TokenType_LiteralInt:
            return "Literal int";
        case TokenType_LiteralFloat:
            return "Literal float";
        case TokenType_Keyword_If:
            return "Keyword if";
        case TokenType_Keyword_Else:
            return "Keyword else";
        case TokenType_Keyword_Int:
            return "Keyword int";
        case TokenType_Keyword_Float:
            return "Keyword float";
        case TokenType_Keyword_Bool:
            return "Keyword bool";
        case TokenType_Keyword_Void:
            return "Keyword void";
        case TokenType_Operator:
            return "Operator";
        case TokenType_Operator_Assign:
            return "Operator equals";
        case TokenType_Brackets_Open:
            return "Brackets open";
        case TokenType_Brackets_Close:
            return "Brackets close";
        case TokenType_Parentheses_Open:
            return "Parentheses open";
        case TokenType_Parentheses_Close:
            return "Parentheses close";
        case TokenType_Braces_Open:
            return "Braces open";
        case TokenType_Braces_Close:
            return "Braces close";
        case TokenType_Semicolon:
            return "Semicolon";
        case TokenType_Comma:
            return "Comma";
        case TokenType_Unknown:
            return "Unknown";
    }
}


std::ostream& operator<<(std::ostream& os, const TokenType& tokenType) {
    os << toString(tokenType);
    return os;
}


TokenType getIfKeyword(const std::string& potentialKeyword) {
    // static const std::string KEYWORDS[] = {"if", "else", "void", "bool", "int", "float"};

    if (potentialKeyword.compare("if") == 0) {
        return TokenType_Keyword_If;
    } else if (potentialKeyword.compare("else") == 0) {
        return TokenType_Keyword_Else;
    } else if (potentialKeyword.compare("void") == 0) {
        return TokenType_Keyword_Void;
    } else if (potentialKeyword.compare("bool") == 0) {
        return TokenType_Keyword_Bool;
    } else if (potentialKeyword.compare("int") == 0) {
        return TokenType_Keyword_Int;
    } else if (potentialKeyword.compare("float") == 0) {
        return TokenType_Keyword_Float;
    }

    return TokenType_Unknown;
}
