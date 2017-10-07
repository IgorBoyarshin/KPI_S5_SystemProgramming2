#ifndef TABLEROW_H
#define TABLEROW_H

#include <string>
#include <ostream>
#include "TableKey.h"

struct TableRow {
    public:
        // Empty row
        TableRow() :
            m_TableKey(TableKey(std::string(""), 0)),
            m_Field(0), m_Deleted(false) {}
        TableRow(
                const TableKey& tableKey,
                double field) :
            m_TableKey(tableKey), m_Field(field), m_Deleted(false) {}
        TableRow(
                const std::string& tableKeyStr,
                unsigned long tableKeyNum,
                double field) :
            m_TableKey(TableKey(tableKeyStr, tableKeyNum)),
            m_Field(field), m_Deleted(false) {}

        // copy constructor
        // TableRow(const TableRow& tableRow) :
        //     m_TableKey(tableRow.getTableKey()),
        //     m_Field(tableRow.getField()),
        //     m_Deleted(tableRow.isDeleted()) {}
    public:
        TableKey m_TableKey;
        double m_Field;
        bool m_Deleted;

    public:
        bool isEmpty() const {
            return m_TableKey.str.size() == 0;
        }

        void markAsDeleted() {
            m_Deleted = true;
        }

        bool isDeleted() const {
            return m_Deleted;
        }

        TableKey getTableKey() const {
            return m_TableKey;
        }

        const std::string& getTableKeyStr() const {
            return m_TableKey.str;
        }

        unsigned long getTableKeyNum() const {
            return m_TableKey.num;
        }

        double getField() const {
            return m_Field;
        }

        // TableRow& operator=(const TableRow& tableRow) {
        //     this->m_TableKey = tableRow.getTableKey();
        //     this->m_Field = tableRow.getField();
        //     this->m_Deleted = tableRow.isDeleted();
        // }
};

std::ostream& operator<<(std::ostream& stream, const TableRow& tableRow);

#endif
