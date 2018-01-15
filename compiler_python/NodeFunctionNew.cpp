#include "NodeFunctionNew.h"


NodeFunctionNew::NodeFunctionNew(
        const std::string name, const std::vector<Argument> arguments)
    : Node(NodeType::FunctionNew), name(name), arguments(arguments) {}
