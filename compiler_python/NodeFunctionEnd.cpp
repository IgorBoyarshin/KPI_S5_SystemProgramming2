#include "NodeFunctionEnd.h"


NodeFunctionEnd::NodeFunctionEnd(
        const std::string& name, const unsigned int amountOfArguments)
    : Node(NodeType::FunctionEnd), name(name), amountOfArguments(amountOfArguments) {}
