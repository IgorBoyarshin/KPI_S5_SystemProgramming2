#include "TokenName.h"

std::ostream& operator<<(std::ostream& os, const TokenName& tokenName) {
    switch(tokenName) {
        case TokenName_Identifier:
            os << "Identifier";
            break;
        case TokenName_Literal:
            os << "Literal";
            break;
        case TokenName_Keyword_If:
            os << "Keyword if";
            break;
        case TokenName_Keyword_Else:
            os << "Keyword else";
            break;
        case TokenName_Operator:
            os << "Operator";
            break;
        case TokenName_Operator_Assign:
            os << "Operator equals";
            break;
        case TokenName_Brackets_Open:
            os << "Brackets open";
            break;
        case TokenName_Brackets_Close:
            os << "Brackets close";
            break;
        case TokenName_Parentheses_Open:
            os << "Parentheses open";
            break;
        case TokenName_Parentheses_Close:
            os << "Parentheses close";
            break;
        case TokenName_Braces_Open:
            os << "Braces open";
            break;
        case TokenName_Braces_Close:
            os << "Braces close";
            break;
        case TokenName_Semicolon:
            os << "Semicolon";
            break;
        case TokenName_Comma:
            os << "Comma";
            break;
        case TokenName_Unknown:
            os << "Unknown";
            break;
    }

    return os;
}


TokenName getIfKeyword(const std::string& potentialKeyword) {
    static const std::string KEYWORDS[] = {"if", "else"};

    if (potentialKeyword.compare("if") == 0) {
        return TokenName_Keyword_If;
    } else if (potentialKeyword.compare("else") == 0) {
        return TokenName_Keyword_Else;
    }

    return TokenName_Unknown;
}
