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
    const NodeType nodeType = node.m_NodeType;
    const std::string nodeValue = node.m_NodeValue;
    // const RuleType ruleType = node.m_RuleType;
    const std::vector<const Node*> children = node.m_Children;

    switch(nodeType) {
        case NodeType_IdUndefined:
        case NodeType_LiteralBool:
            os << nodeValue;
            break;

        case NodeType_KeywordIf:
            os << "if";
            break;
        case NodeType_KeywordElse:
            os << "else";
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
        case NodeType_StatementList:
        case NodeType_ExpressionBool:
        case NodeType_FunctionCallVoid:
        case NodeType_ExpressionList:
            for (const Node* node : children) {
                os << *node;
            }
            break;

        default:
            std::cout << ":> ERROR: Unexpected NodeType: "
                << nodeValue;
    }

    return os;
}

