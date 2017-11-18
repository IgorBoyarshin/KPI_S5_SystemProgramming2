#ifndef CONTEXTNAME_H
#define CONTEXTNAME_H

#include <iostream>
#include <string>

enum ContextName {
    ContextName_IfStatement,
    ContextName_StatementBody,
    ContextName_Function
};

std::ostream& operator<<(std::ostream& os, const ContextName& contextName);

#endif
