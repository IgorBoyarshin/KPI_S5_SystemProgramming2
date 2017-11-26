#include "Node.h"
#include <ostream>
#include <iostream>

/*
 * switch(NodeType):
 *  Terminals:
 *      value = -- (can be derived)
 *      rule = --
 *      children = --
 *  Id, Literal:
 *      value = string for id/literal
 *      rule = --
 *      children = --
 *  Non-terminals:
 *      value = --
 *      rule = certain rule
 *      children = children to reconstruct the node
 *
 */

Node::Node(
        const NodeType nodeType
    ) :
    m_NodeType(nodeType),
    m_NodeValue(std::string()),
    m_RuleType(RuleType_Statement_Semicolon),
    m_Children(std::vector<const Node*>()) {}

Node::Node(
        const NodeType nodeType,
        const RuleType ruleType,
        const std::vector<const Node*>& children
    ) :
    m_NodeType(nodeType),
    m_NodeValue(std::string()),
    m_RuleType(ruleType),
    m_Children(children) {}

Node::Node(
        const NodeType nodeType,
        const std::string& nodeValue
    ) :
    m_NodeType(nodeType),
    m_NodeValue(nodeValue),
    m_RuleType(RuleType_Statement_Semicolon),
    m_Children(std::vector<const Node*>()) {}


std::ostream& operator<<(std::ostream& os, const Node& node) {
    static unsigned int shiftLevel = 0;

    const NodeType nodeType = node.m_NodeType;
    const std::string nodeValue = node.m_NodeValue;
    // const RuleType ruleType = node.m_RuleType;
    const std::vector<const Node*> children = node.m_Children;


    for (unsigned int i = 0; i < shiftLevel; i++) {
        std::cout << "|   ";
    }
    std::cout << nodeType << std::endl;
    switch (nodeType) {
        case NodeType_Id:
        case NodeType_IdVoid:
        case NodeType_IdBool:
        case NodeType_IdInt:
        case NodeType_IdFloat:
        case NodeType_LiteralBool:
        case NodeType_LiteralInt:
        case NodeType_LiteralFloat:
            shiftLevel++;
            for (unsigned int i = 0; i < shiftLevel; i++) {
                std::cout << "|   ";
            }
            std::cout << nodeValue << std::endl;
            shiftLevel--;
            break;

        default:
            break;
    }
    shiftLevel++;

    for (const Node* node : children) {
        os << *node;
    }

    shiftLevel--;
    // for (unsigned int i = 0; i < shiftLevel; i++) {
    //     std::cout << "|   ";
    // }
    // std::cout << "--" << std::endl;






    // std::cout << nodeType << " ";
    // switch(nodeType) {
    //     case NodeType_IdUndefined:
    //     case NodeType_Id:
    //         os << "? " << nodeValue;
    //         break;
    //     case NodeType_IdVoid:
    //         os << "void " << nodeValue;
    //         break;
    //     case NodeType_IdBool:
    //         os << "bool " << nodeValue;
    //         break;
    //     case NodeType_IdInt:
    //         os << "int " << nodeValue;
    //         break;
    //     case NodeType_IdFloat:
    //         os << "float " << nodeValue;
    //         break;
    //     case NodeType_LiteralBool:
    //         os << "L bool " << nodeValue;
    //         break;
    //     case NodeType_LiteralInt:
    //         os << "L int " << nodeValue;
    //         break;
    //     case NodeType_LiteralFloat:
    //         os << "L float " << nodeValue;
    //         break;
    //
    //     case NodeType_KeywordIf:
    //         os << "if";
    //         break;
    //     case NodeType_KeywordElse:
    //         os << "else";
    //         break;
    //     case NodeType_KeywordVoid:
    //         os << "void";
    //         break;
    //     case NodeType_KeywordBool:
    //         os << "bool";
    //         break;
    //     case NodeType_KeywordInt:
    //         os << "int";
    //         break;
    //     case NodeType_KeywordFloat:
    //         os << "float";
    //         break;
    //     case NodeType_BracketsOpen:
    //         os << "[";
    //         break;
    //     case NodeType_BracketsClose:
    //         os << "]";
    //         break;
    //     case NodeType_ParenthesesOpen:
    //         os << "(";
    //         break;
    //     case NodeType_ParenthesesClose:
    //         os << ")";
    //         break;
    //     case NodeType_BracesOpen:
    //         os << "{";
    //         break;
    //     case NodeType_BracesClose:
    //         os << "}";
    //         break;
    //     case NodeType_OperatorPlus:
    //         os << "+";
    //         break;
    //     case NodeType_OperatorMinus:
    //         os << "-";
    //         break;
    //     case NodeType_OperatorMultiply:
    //         os << "*";
    //         break;
    //     case NodeType_OperatorDivide:
    //         os << "/";
    //         break;
    //     case NodeType_OperatorEquals:
    //         os << "==";
    //         break;
    //     case NodeType_OperatorNotEquals:
    //         os << "!=";
    //         break;
    //     case NodeType_OperatorLess:
    //         os << "<";
    //         break;
    //     case NodeType_OperatorLessOrEquals:
    //         os << "<=";
    //         break;
    //     case NodeType_OperatorGreater:
    //         os << ">";
    //         break;
    //     case NodeType_OperatorGreaterOrEquals:
    //         os << ">=";
    //         break;
    //     case NodeType_OperatorAssign:
    //         os << "=";
    //         break;
    //     case NodeType_Comma:
    //         os << ",";
    //         break;
    //     case NodeType_Semicolon:
    //         os << ";";
    //         break;
    //
    //     case NodeType_Block:
    //     case NodeType_TypedIdList:
    //     case NodeType_TypedId:
    //     case NodeType_DeclarationList:
    //     case NodeType_Declaration:
    //     case NodeType_StatementList:
    //     case NodeType_Statement:
    //     case NodeType_ExpressionList:
    //     case NodeType_ExpressionBool:
    //     case NodeType_ExpressionInt:
    //     case NodeType_ExpressionFloat:
    //     case NodeType_FunctionCallVoid:
    //     case NodeType_FunctionCallBool:
    //     case NodeType_FunctionCallInt:
    //     case NodeType_FunctionCallFloat:
    //         for (const Node* node : children) {
    //             os << "--" << *node << std::endl;
    //         }
    //         break;
    //
    //     default:
    //         std::cout << ":> ERROR: Unexpected NodeType: "
    //             << nodeValue;
    // }

    return os;
}

