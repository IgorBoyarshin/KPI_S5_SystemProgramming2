#ifndef NODETYPE_H
#define NODETYPE_H

#include <string>
#include <iostream>

enum NodeType {
    // Terminal symbols. Runtime-defined
    NodeType_Id,
    NodeType_IdUndefined,
    NodeType_IdVoid,
    NodeType_IdBool,
    NodeType_IdInt,
    NodeType_IdFloat,
    NodeType_IdPointerBool,
    NodeType_IdPointerInt,
    NodeType_IdPointerFloat,
    NodeType_LiteralBool,
    NodeType_LiteralInt,
    NodeType_LiteralFloat,


    // Terminal symbols. Compiletime-defined
    NodeType_KeywordIf,
    NodeType_KeywordElse,
    NodeType_KeywordVoid,
    NodeType_KeywordBool,
    NodeType_KeywordInt,
    NodeType_KeywordFloat,
    NodeType_KeywordPointerBool,
    NodeType_KeywordPointerInt,
    NodeType_KeywordPointerFloat,
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
    NodeType_Block,
    NodeType_TypedIdList,
    NodeType_TypedId,
    NodeType_DeclarationList,
    NodeType_Declaration,
    NodeType_StatementList,
    NodeType_Statement,
    NodeType_ExpressionList,
    NodeType_ExpressionBool,
    NodeType_ExpressionInt,
    NodeType_ExpressionFloat,
    NodeType_ExpressionPointerBool,
    NodeType_ExpressionPointerInt,
    NodeType_ExpressionPointerFloat,
    NodeType_FunctionCallVoid,
    NodeType_FunctionCallBool,
    NodeType_FunctionCallInt,
    NodeType_FunctionCallFloat,
    NodeType_FunctionCallPointerBool,
    NodeType_FunctionCallPointerInt,
    NodeType_FunctionCallPointerFloat,
};


NodeType getNodeTypeFromString(const std::string& str);
std::string toString(NodeType nodeType);

std::ostream& operator<<(std::ostream& os, const NodeType& nodeType);

#endif
