#ifndef NODETYPE_H
#define NODETYPE_H

#include <string>
#include <iostream>

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
    NodeType_OperatorLess,
    NodeType_OperatorLessOrEquals,
    NodeType_OperatorGreater,
    NodeType_OperatorGreaterOrEquals,
    NodeType_OperatorAssign,
    NodeType_Comma,
    NodeType_Semicolon,

    // Non-terminal symbols. Compiletime-defined
    // NodeType_IfBlock,
    // NodeType_IfElseBlock,
    NodeType_Statement,
    NodeType_Expression,
    NodeType_Function,
    NodeType_StatementList,
    NodeType_ExpressionList
};

std::ostream& operator<<(std::ostream& os, const NodeType& nodeType);
unsigned int stringToInt(const std::string& str);

#endif
