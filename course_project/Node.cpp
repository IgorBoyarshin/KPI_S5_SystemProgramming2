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
    m_Children(std::vector<const Node*>()) {}

Node::Node(
        const NodeType nodeType,
        const std::vector<const Node*>& children
    ) :
    m_NodeType(nodeType),
    m_NodeValue(std::string()),
    m_Children(children) {}

Node::Node(
        const NodeType nodeType,
        const std::string& nodeValue
    ) :
    m_NodeType(nodeType),
    m_NodeValue(nodeValue),
    m_Children(std::vector<const Node*>()) {}


std::ostream& operator<<(std::ostream& os, const Node& node) {
    static unsigned int shiftLevel = 0;

    const NodeType nodeType = node.m_NodeType;
    const std::string nodeValue = node.m_NodeValue;
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

    return os;
}

