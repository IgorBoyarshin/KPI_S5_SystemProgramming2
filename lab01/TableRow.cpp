#include "TableRow.h"

std::ostream& operator<<(std::ostream& stream, const TableRow& tableRow) {
    stream << "TableRow { "
        << "keyStr: \"" << tableRow.getTableKeyStr()
        << "\"; keyNum: " << tableRow.getTableKeyNum()
        << "; field: " << tableRow.getField()
        << "; }";

    return stream;
}
