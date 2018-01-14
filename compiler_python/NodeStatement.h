#ifndef NODESTATEMENT_H
#define NODESTATEMENT_H

#include "Node.h"
#include <string>


class NodeStatement : public Node {
    public:
        const std::string code;

        NodeStatement(std::string code);
};


#endif
