#ifndef NODE_H
#define NODE_H

#include <vector>
#include <string>
#include "NodeType.h"

class Node {
    public:
        Node(
            const NodeType nodeType
        );

        Node(
            const NodeType nodeType,
            const std::vector<const Node*>& children
        );

        Node(
            const NodeType nodeType,
            const std::string& nodeValue
        );

    public:
        NodeType m_NodeType;
        std::string m_NodeValue;
        std::vector<const Node*> m_Children;

        friend std::ostream& operator<<(std::ostream& os, const Node& node);


    public:
        // NodeType getNodeType() {
        //     return m_NodeType;
        // }
        //
        // std::string getNodeValue() {
        //     return m_NodeValue;
        // }

};

std::ostream& operator<<(std::ostream& os, const Node& node);

#endif
