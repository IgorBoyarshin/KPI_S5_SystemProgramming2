#include "TokenType.h"


std::ostream& operator<<(std::ostream& os, const TokenType& tokenType) {
    os << toString(tokenType);
    return os;
}


std::string toString(const TokenType& tokenType) {
    switch (tokenType) {
        case TokenType_NewLine:
            return "NEWLINE";
        case TokenType_Tab:
            return "TAB";
        case TokenType_Identifier:
            return "IDENTIFIER";
        case TokenType_Number:
            return "NUMBER";
        case TokenType_Bool:
            return "BOOL";
        case TokenType_LeftParenthesis:
            return "(";
        case TokenType_RightParenthesis:
            return ")";
        case TokenType_Comma:
            return ",";
        case TokenType_Colon:
            return ":";
        case TokenType_Semicolon:
            return ";";
        case TokenType_Assign:
            return "=";
        case TokenType_Plus:
            return "+";
        case TokenType_Minus:
            return "-";
        case TokenType_Multiply:
            return "*";
        case TokenType_Divide:
            return "/";
        case TokenType_Greater:
            return ">";
        case TokenType_Less:
            return "<";
        case TokenType_GreaterOrEquals:
            return ">=";
        case TokenType_LessOrEquals:
            return "<=";
        case TokenType_KeywordAnd:
            return "AND";
        case TokenType_KeywordOr:
            return "OR";
        case TokenType_Equals:
            return "==";
        case TokenType_KeywordDef:
            return "DEF";
        case TokenType_KeywordReturn:
            return "RETURN";
        case TokenType_KeywordFor:
            return "FOR";
        case TokenType_KeywordIn:
            return "IN";
        case TokenType_KeywordRange:
            return "RANGE";
        case TokenType_KeywordWhile:
            return "WHILE";

        default:
            return "???";
    }
}


TokenType getIfKeyword(const std::string& potentialKeyword) {
    if (potentialKeyword.compare("for") == 0) {
        return TokenType_KeywordFor;
    } else if (potentialKeyword.compare("in") == 0) {
        return TokenType_KeywordIn;
    } else if (potentialKeyword.compare("range") == 0) {
        return TokenType_KeywordRange;
    } else if (potentialKeyword.compare("while") == 0) {
        return TokenType_KeywordWhile;
    } else if (potentialKeyword.compare("def") == 0) {
        return TokenType_KeywordDef;
    } else if (potentialKeyword.compare("return") == 0) {
        return TokenType_KeywordReturn;
    } else if (potentialKeyword.compare("and") == 0) {
        return TokenType_KeywordAnd;
    } else if (potentialKeyword.compare("or") == 0) {
        return TokenType_KeywordOr;
    }

    return TokenType_Unknown;
}
