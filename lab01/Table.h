#ifndef TABLE_H
#define TABLE_H

#include <vector>
#include <ostream>
#include "TableRow.h"


class Table {
    public:
        Table() : m_TableRows(std::vector<TableRow>()), m_Length(0) {};
        Table(std::vector<TableRow>& tableRows) :
            m_TableRows(tableRows), m_Length(0) {};

    private:
        std::vector<TableRow> m_TableRows;
        unsigned short m_Length;

    public:
        bool isPresent(const TableKey& tableKey) const;
        bool isPresent(
                const std::string tableKeyStr,
                unsigned long tableKeyNum) const;

        const TableRow& selectDirect(unsigned short index) const;

        // returns true if insertion was successful, false otherwise
        bool insertDirect(const TableRow& tableRow, unsigned short index);

        // returns true if the deletion was valid, false otherwise
        bool deleteDirect(unsigned short index);

        void updateDirect(
                const TableRow& newTableRow, unsigned short index);


        TableRow* selectLinear(const TableKey& tableKey);
        void insertLinear(const TableRow& tableRow);
        void deleteLinear(const TableRow& tableRow);
        TableRow* updateLinear(
                const TableKey& tableKey, const TableRow& newTableRow);
        void packLinear();
        void sort();


        TableRow* selectBinary(const TableKey& tableKey);
        void insertBinary(const TableRow& tableRow);
        void deleteBinary(const TableRow& tableRow);
        TableRow* updateBinary(
                const TableKey& tableKey, const TableRow& newTableRow);

        TableRow* selectBestFit(const TableKey& tableKey);


        friend std::ostream& operator<<(
                std::ostream& stream, const Table& table);

};

std::ostream& operator<<(std::ostream& stream, const Table& table);

bool isEqual(const TableRow& tableRow, const TableKey& tableKey);
int compare(const std::string& string1, const std::string& string2);
int compare(const TableKey& tableKey1, const TableKey& tableKey2);
int compare(const TableRow& tableRow, const TableKey& tableKey);
int compareSimilarity(const TableKey& tk1, const TableKey& tk2);
int compareSimilarity(const std::string& tk1, const std::string& tk2);

// English <-> Russian(Ukr)
typedef std::pair<std::string, std::string> LetterPair;
inline const std::vector< LetterPair >& getSimilarLetters();
bool areSimilar(const std::string& a, const std::string& b);
bool isAmongSimilarLetters(const std::string& l);

#endif
