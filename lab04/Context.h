#ifndef CONTEXT_H
#define CONTEXT_H

#include "ContextType.h"

class Context {
    public:
        ContextType type;
        unsigned int counter;

        bool isInFunction() const;
        bool isInIfStatement() const;
        bool isInStatementBody() const;
        bool isInExpression() const;
        bool isIn(const ContextType contextType) const;
};

#endif
