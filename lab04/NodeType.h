#ifndef NODETYPE_H
#define NODETYPE_H

#include <string>

enum NodeType {
    // Terminal symbols. Runtime-defined
    NodeType_Id,
    NodeType_Literal,

    // Terminal symbols. Compiletime-defined
    NodeType_KeywordIf,
    NodeType_KeywordElse,
    NodeType_BracketsOpen, // [
    NodeType_BracketsClose, // ]
    NodeType_ParenthesesOpen, // (
    NodeType_ParenthesesClose, // )
    NodeType_BracesOpen, // {
    NodeType_BracesClose, // }
    NodeType_OperatorPlus,
    NodeType_OperatorMinus,
    NodeType_OperatorMultiply,
    NodeType_OperatorDivide,
    NodeType_OperatorEquals,
    NodeType_OperatorNotEquals,
    NodeType_OperatorAssign,
    NodeType_Comma,
    NodeType_Semicolon,

    // Non-terminal symbols. Compiletime-defined
    // NodeType_IfBlock,
    // NodeType_IfElseBlock,
    NodeType_Statement,
    NodeType_StatementBody,
    NodeType_Expression,
    NodeType_Function
};

// Caution: not generalized
unsigned int stringToInt(const std::string& str) {
    unsigned int number = 0;
    const unsigned int length = str.size();
    for (unsigned int i = 0; i < length; i++) {
        number = 10 * number + str[length - i - 1];
    }

    return number;
}

#endif
