#include "Type.h"


std::ostream& operator<<(std::ostream& os, const Type& type) {
    switch (type) {
        case Type_Number:
            os << "Type_Number";
            break;
        case Type_Bool:
            os << "Type_Bool";
            break;
        case Type_FuncNumber:
            os << "Type_FuncNumber";
            break;
        case Type_FuncBool:
            os << "Type_FuncBool";
            break;
    }

    return os;
}
