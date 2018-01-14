#include "PieceType.h"


std::ostream& operator<<(std::ostream& os, PieceType pieceType) {
    switch (pieceType) {
        case PieceType_Comma:
            os << "PieceType_Comma";
            break;
        case PieceType_ParenthesesOpen:
            os << "PieceType_ParenthesesOpen";
            break;
        case PieceType_ParenthesesClose:
            os << "PieceType_ParenthesesClose";
            break;
        case PieceType_Plus:
            os << "PieceType_Plus";
            break;
        case PieceType_Minus:
            os << "PieceType_Minus";
            break;
        case PieceType_Multiply:
            os << "PieceType_Multiply";
            break;
        case PieceType_Divide:
            os << "PieceType_Divide";
            break;
        case PieceType_Greater:
            os << "PieceType_Greater";
            break;
        case PieceType_GreaterOrEquals:
            os << "PieceType_GreaterOrEquals";
            break;
        case PieceType_Less:
            os << "PieceType_Less";
            break;
        case PieceType_LessOrEquals:
            os << "PieceType_LessOrEquals";
            break;
        case PieceType_Equals:
            os << "PieceType_Equals";
            break;
        case PieceType_And:
            os << "PieceType_And";
            break;
        case PieceType_Or:
            os << "PieceType_Or";
            break;
        case PieceType_Bool:
            os << "PieceType_Bool";
            break;
        case PieceType_Number:
            os << "PieceType_Number";
            break;
        case PieceType_FunctionBool:
            os << "PieceType_FunctionBool";
            break;
        case PieceType_FunctionNumber:
            os << "PieceType_FunctionNumber";
            break;
        case PieceType_FunctionEnd:
            os << "PieceType_FunctionEnd";
            break;
        default:
            os << "PieceType_Undefined";
            break;
    }

    return os;
}


PieceType getOperatorResultType(PieceType pieceType) {
    switch (pieceType) {
        case PieceType_Plus:
        case PieceType_Minus:
        case PieceType_Multiply:
        case PieceType_Divide:
            return PieceType_Number;

        case PieceType_Greater:
        case PieceType_GreaterOrEquals:
        case PieceType_Less:
        case PieceType_LessOrEquals:
        case PieceType_Equals:
        case PieceType_And:
        case PieceType_Or:
            return PieceType_Bool;

        default:
            std::cout << "Internal error(shouldn't have called this method with such argument)"
                      << std::endl;
            return PieceType_Undefined;
    }
}
