#ifndef PIECETYPE_H
#define PIECETYPE_H

#include <iostream>


enum PieceType {
    PieceType_Comma, PieceType_ParenthesesOpen, PieceType_ParenthesesClose,
    PieceType_Plus, PieceType_Minus, PieceType_Multiply, PieceType_Divide,
    PieceType_Greater, PieceType_GreaterOrEquals,
    PieceType_Less, PieceType_LessOrEquals,
    PieceType_Equals, PieceType_And, PieceType_Or,
    PieceType_Bool, PieceType_Number,
    PieceType_FunctionBool, PieceType_FunctionNumber, PieceType_FunctionEnd,
    PieceType_Undefined
};


std::ostream& operator<<(std::ostream& os, PieceType pieceType);
PieceType getOperatorResultType(PieceType pieceType);


#endif

