#ifndef NODEFUNCTIONEND_H
#define NODEFUNCTIONEND_H

#include <string>
#include <vector>
#include "Node.h"
#include "NodeType.h"
#include "Argument.h"


class NodeFunctionEnd : public Node {
    public:
        const std::string name;
        const unsigned int amountOfArguments;
        NodeFunctionEnd(const std::string& name, const unsigned int amountOfArguments);
};


#endif
