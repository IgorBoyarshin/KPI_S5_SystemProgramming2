#ifndef NODEFUNCTIONNEW_H
#define NODEFUNCTIONNEW_H

#include <string>
#include <vector>
#include "Node.h"
#include "NodeType.h"
#include "Argument.h"


class NodeFunctionNew : public Node {
    public:
        const std::string name;
        const std::vector<Argument> arguments;
        NodeFunctionNew(const std::string name, const std::vector<Argument> arguments);
};


#endif
