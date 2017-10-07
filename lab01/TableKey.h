#ifndef TABLEKEY_H
#define TABLEKEY_H

#include <string>

struct TableKey {
    public:
        std::string str;
        unsigned long num;

        TableKey(const std::string& str, unsigned long num) :
            str(str), num(num) {};

        // empty key
        TableKey() : str(std::string("")), num(0) {};

        // copy constructor
        // TableKey(const TableKey& tableKey) :
        //     str(tableKey.str), num(tableKey.num) {}
};

#endif
