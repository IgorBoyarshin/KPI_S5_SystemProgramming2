#include "TokenName.h"

std::ostream& operator<<(std::ostream& os, const TokenName& tokenName) {
    switch(tokenName) {
        case TokenName_Identifier:
            os << "Identifier";
            break;
        case TokenName_Keyword:
            os << "Keyword";
            break;
        case TokenName_Operator:
            os << "Operator";
            break;
        case TokenName_Brackets:
            os << "Brackets";
            break;
        case TokenName_Parentheses:
            os << "Parentheses";
            break;
        case TokenName_Braces:
            os << "Braces";
            break;
        case TokenName_Literal:
            os << "Literal";
            break;
        case TokenName_Semicolon:
            os << "Semicolon";
            break;
        case TokenName_Unknown:
            os << "Unknown";
            break;
    }

    return os;
}


bool isKeyword(const std::string& potentialKeyword) {
    static const std::string KEYWORDS[] = {"if", "else", "sin"};

    for (const std::string& keyword : KEYWORDS) {
        if (potentialKeyword.compare(keyword) == 0) {
            return true;
        }
    }

    return false;
}
