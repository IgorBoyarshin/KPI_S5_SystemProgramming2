#include "Context.h"


bool Context::isIn(const ContextType contextType) const {
    return (type == contextType  && counter > 0);
}


bool Context::isInIfStatement() const {
    return isIn(ContextType_IfStatement);
}

bool Context::isInStatementBody() const {
    return isIn(ContextType_StatementBody);
}

bool Context::isInExpression() const {
    return isIn(ContextType_Expression);
}

bool Context::isInFunction() const {
    return isIn(ContextType_Function);
}
