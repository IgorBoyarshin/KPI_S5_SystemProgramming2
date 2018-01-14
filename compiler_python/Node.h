#ifndef NODE_H
#define NODE_H

#include "NodeType.h"


class Node {
    public:
        const NodeType type;

        Node(NodeType nodeType);
};


#endif
