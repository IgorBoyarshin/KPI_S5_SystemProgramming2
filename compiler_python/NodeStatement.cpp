#include "NodeStatement.h"


NodeStatement::NodeStatement(std::string code) :
    Node(NodeType::Statement), code(code) {}
