#ifndef TYPE_H
#define TYPE_H

#include <iostream>


enum Type {
    Type_Number,
    Type_Bool,
    Type_FuncNumber,
    Type_FuncBool
};

std::ostream& operator<<(std::ostream& os, const Type& type);


#endif
