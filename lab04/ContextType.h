#ifndef CONTEXTTYPE_H
#define CONTEXTTYPE_H

#include <iostream>
#include <string>

enum ContextType {
    ContextType_IfStatement,
    ContextType_StatementBody,
    ContextType_Expression,
    ContextType_Function
};

std::ostream& operator<<(std::ostream& os, const ContextType& contextType);

#endif
