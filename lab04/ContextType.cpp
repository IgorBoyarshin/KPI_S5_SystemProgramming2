#include "ContextType.h"

std::ostream& operator<<(std::ostream& os, const ContextType& contextType) {
    switch (contextType) {
        case ContextType_IfStatement:
            os << "If statement";
            break;
        case ContextType_StatementBody:
            os << "Statement body";
            break;
        case ContextType_Function:
            os << "Function";
            break;
        default:
            os << ":> Error: Undefined ContextType";
    }

    return os;
}
