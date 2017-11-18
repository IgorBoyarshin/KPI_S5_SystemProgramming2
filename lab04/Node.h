#ifndef NODE_H
#define NODE_H

#include <vector>
#include <string>
#include "NodeType.h"

class Node {
    public:
        Node(const NodeType nodeType, const std::string& nodeValue,
                const std::vector<Node*>& children);

    public:
        NodeType m_NodeType;
        std::string m_NodeValue;
        std::vector<Node*> m_Children;

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
