#ifndef ARGUMENT_H
#define ARGUMENT_H

#include <string>
#include "Type.h"


class Argument {
    public:
        const std::string name;
        const Type type;
        Argument(const std::string name, Type type);
};

#endif
