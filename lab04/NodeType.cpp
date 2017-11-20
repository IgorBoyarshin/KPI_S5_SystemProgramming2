#include "NodeType.h"

// Caution: not generalized
unsigned int stringToInt(const std::string& str) {
    unsigned int number = 0;
    const unsigned int length = str.size();
    for (unsigned int i = 0; i < length; i++) {
        number = 10 * number + str[length - i - 1];
    }

    return number;
}

std::ostream& operator<<(std::ostream& os, const NodeType& nodeType) {
    switch(nodeType) {
        case NodeType_Id:
            os << "ID";
            break;
        case NodeType_Literal:
            os << "LITERAL";
            break;
        case NodeType_KeywordIf:
            os << "IF";
            break;
        case NodeType_KeywordElse:
            os << "ELSE";
            break;
        case NodeType_BracketsOpen:
            os << "[";
            break;
        case NodeType_BracketsClose:
            os << "]";
            break;
        case NodeType_ParenthesesOpen:
            os << "(";
            break;
        case NodeType_ParenthesesClose:
            os << ")";
            break;
        case NodeType_BracesOpen:
            os << "{";
            break;
        case NodeType_BracesClose:
            os << "}";
            break;
        case NodeType_OperatorPlus:
            os << "+";
            break;
        case NodeType_OperatorMinus:
            os << "-";
            break;
        case NodeType_OperatorMultiply:
            os << "*";
            break;
        case NodeType_OperatorDivide:
            os << "/";
            break;
        case NodeType_OperatorEquals:
            os << "==";
            break;
        case NodeType_OperatorNotEquals:
            os << "!=";
            break;
        case NodeType_OperatorAssign:
            os << "=";
            break;
        case NodeType_Comma:
            os << ",";
            break;
        case NodeType_Semicolon:
            os << ";";
            break;

        case NodeType_Statement:
            os << "STATEMENT";
            break;

        case NodeType_Expression:
            os << "EXPRESSION";
            break;

        case NodeType_Function:
            os << "FUNCTION";
            break;
        case NodeType_ExpressionList:
            os << "EXPRESSION_LIST";
            break;
        case NodeType_StatementList:
            os << "STATEMENT_LIST";
            break;

        default:
            std::cout << ":> ERROR: Unexpected NodeType: ";
    }

    return os;
}
