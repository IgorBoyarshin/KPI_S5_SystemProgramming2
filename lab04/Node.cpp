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
        case NodeType_Id:
        case NodeType_Literal:
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
        case NodeType_Expression:
        case NodeType_Function:
        case NodeType_ExpressionList:
            for (const Node* node : children) {
                os << *node;
            }
            break;

        //     switch (ruleType) {
        //         case RuleType_Statement_Function: // <Function>;
        //             // os << children[0] << ";";
        //             // break;
        //         case RuleType_Statement_IdEqExpression: // <Id> = <Expression>;
        //             os << children[0] << " = " << children[1];
        //             break;
        //         case RuleType_Statement_Semicolon:
        //             os << ";";
        //             break;
        //         case RuleType_Statement_Braces:
        //             os << "{}";
        //             break;
        //         case RuleType_Statement_BracesBlock:
        //             os << "{";
        //             os << children[0];
        //             os << "}";
        //             break;
        //         case 3: // if (<Expression>) <Statement>
        //             os << "if" << "("
        //                << children[0]
        //                << ")"
        //                << children[1];
        //             break;
        //         case 4: // if (<Expression>) <Statement> else <Statement>
        //             os << "if" << "("
        //                << children[0]
        //                << ")"
        //                << children[1]
        //                << "else"
        //                << children[2];
        //             break;
        //         default:
        //             std::cout
        //                 << ":> ERROR: Unexpected switch case for <Statement>: "
        //                 << nodeValue << std::endl;
        //     }
        //     break;
        //
        // case NodeType_Expression:
        //     switch (stringToInt(nodeValue)) {
        //         case 1: // <Id>
        //         case 2: // <Literal>
        //         case 3: // <Function>
        //             os << children[0];
        //             break;
        //         case 4: // <Expression> <Operator> <Expression>
        //             os << children[0] << children[1] << children[2];
        //             break;
        //         case 5: // ( <Expression> )
        //             os << "(" << children[0] << ")";
        //             break;
        //         default:
        //             std::cout
        //                 << ":> ERROR: Unexpected switch case for <Expression>: "
        //                 << nodeValue << std::endl;
        //     }
        //     break;
        //
        // case NodeType_Function:
        //     switch (stringToInt(nodeValue)) {
        //         // TODO: combine??
        //         case 1: // <Id>()
        //             os << children[0] << "()";
        //             break;
        //         case 2: // <Id>(<Expression>^[1..])
        //             os << children[0] << "(";
        //             os << children[1];
        //             for (unsigned int i = 2; i < children.size(); i++) {
        //                 os << "," << children[i];
        //             }
        //             os << ")";
        //             break;
        //         default:
        //             std::cout
        //                 << ":> ERROR: Unexpected switch case for <Function>: "
        //                 << nodeValue << std::endl;
        //     }
        //     break;

        default:
            std::cout << ":> ERROR: Unexpected NodeType: "
                << nodeValue;
    }

    return os;
}

