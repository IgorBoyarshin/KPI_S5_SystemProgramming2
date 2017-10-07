#include <ostream>
#include <iostream>
#include <algorithm>
#include "Table.h"


bool Table::isPresent(const TableKey& tableKey) const {
    for (auto const& tableRow : m_TableRows) {
        if (isEqual(tableRow, tableKey)) {
            return true;
        }
    }

    return false;
}

bool Table::isPresent(
        const std::string tableKeyStr,
        unsigned long tableKeyNum) const {
    return Table::isPresent(TableKey(tableKeyStr, tableKeyNum));
}


const TableRow& Table::selectDirect(unsigned short index) const {
    return m_TableRows[index];
}

bool Table::insertDirect(const TableRow& tableRow, unsigned short index) {
    if (index > m_Length) {
        return false;
    }

    while (index < m_Length) {
        m_TableRows[index++] = TableRow();
    }
    m_TableRows.push_back(tableRow);
    m_Length = ++index;

    return true;
}

bool Table::deleteDirect(unsigned short index) {
    if (index < m_Length) {
        m_TableRows[index].markAsDeleted();
        return true;
    }

    return false;
}

void Table::updateDirect(
        const TableRow& newTableRow, unsigned short index) {
    if (index >= m_Length) {
        return;
    }

    m_TableRows[index] = newTableRow;
}
// ------------------------------------------------------------------ //

TableRow* Table::selectLinear(const TableKey& tableKey) {
    for (auto& tableRow : m_TableRows) {
        if (isEqual(tableRow, tableKey)) {
            return &tableRow;
        }
    }

    return nullptr;
}

void Table::insertLinear(const TableRow& tableRow) {
    unsigned short index;
    for (index = 0; index < m_Length && !tableRow.isEmpty(); index++);

    // reached end, no empty elements found
    if (index == m_Length) {
        m_Length++;
        m_TableRows.push_back(tableRow);
    } else {
        m_TableRows[index] = tableRow;
    }
}

void Table::deleteLinear(const TableRow& tableRow) {
    TableRow* const tableRowToDelete = selectLinear(tableRow.getTableKey());
    if (tableRowToDelete != nullptr) {
        tableRowToDelete->markAsDeleted();
    }
}

TableRow* Table::updateLinear(
        const TableKey& tableKey, const TableRow& newTableRow) {
    TableRow* const tableRowToUpdate = selectLinear(tableKey);
    if (tableRowToUpdate != nullptr) {
        *tableRowToUpdate = newTableRow;
    }

    return tableRowToUpdate;
}

void Table::packLinear() {
    for (unsigned short i = 0; i < m_Length; i++) {
        if (m_TableRows[i].isDeleted()) {
            m_TableRows.erase(m_TableRows.begin() + i);
        }
    }
    m_Length = m_TableRows.size();

    // unsigned short n = 0;
    // unsigned short ln = 0;
    // while (n < m_Length) {
    //     if (m_TableRows[n].isDeleted()) {
    //         m_TableRows[ln] = m_TableRows[n];
    //         ln++;
    //     }
    //     n++;
    // }
}

void Table::sort() {
    std::sort(m_TableRows.begin(), m_TableRows.end(),
            [](const TableRow& tr1, const TableRow& tr2) {
                return compare(tr1.getTableKey(), tr2.getTableKey()) < 0;
            });
}


TableRow* Table::selectBinary(const TableKey& tableKey) {
    sort();

    unsigned short l = 0;
    unsigned short r = m_Length - 1;

    while (r >= l) {
        unsigned short m = (l + r) / 2;

        TableRow* tableRow = &m_TableRows[m];
        int comparisonResult = compare(*tableRow, tableKey);
        if (comparisonResult == 0) {
            return tableRow;
        } else if (comparisonResult > 0) {
            if (m < 1) {
                break;
            }
            r = m - 1;
        } else {
            l = m + 1;
        }
    }

    return nullptr;
}

void Table::insertBinary(const TableRow& tableRow) {
    const TableKey& tableKey = tableRow.getTableKey();
    sort();

    unsigned short l = 0;
    unsigned short r = m_Length - 1;

    unsigned short m;
    int comparisonResult;
    while (r >= l) {
        m = (l + r) / 2;
        comparisonResult = compare(m_TableRows[m], tableKey);
        if (comparisonResult == 0) {
            m_TableRows[m] = tableRow;
            return;
        } else if (comparisonResult > 0) {
            if (m < 1) {
                break;
            }
            r = m - 1;
        } else {
            l = m + 1;
        }
    }

    // No match => insert
    // m contains our best guess,
    // comparisonResult tells us in what direction to insert
    m_TableRows.emplace(m_TableRows.begin() + m +
            (comparisonResult >= 0 ? 0 : 1), tableRow);
}

void Table::deleteBinary(const TableRow& tableRow) {
    TableRow* foundTableRow = selectBinary(tableRow.getTableKey());
    if (foundTableRow != nullptr) {
        foundTableRow->markAsDeleted();
    }
}

TableRow* Table::updateBinary(
        const TableKey& tableKey, const TableRow& newTableRow) {
    TableRow* foundTableRow = selectBinary(tableKey);
    if (foundTableRow != nullptr) {
        *foundTableRow = newTableRow;
    }

    return foundTableRow;
}


TableRow* Table::selectBestFit(const TableKey& tableKey) {
    TableRow* currentBestFit = nullptr;
    int currentMax = -1;

    for (TableRow& tableRow : m_TableRows) {
        const int similarity =
            compareSimilarity(tableRow.getTableKey(), tableKey);
        if (similarity > currentMax) {
            currentMax = similarity;
            currentBestFit = &tableRow;
        }
    }

    return currentBestFit;
}

// ------------------------------------------------------------------ //

std::ostream& operator<<(std::ostream& stream, const Table& table) {
    std::cout << "Table {" << std::endl;
    for (auto const& tableRow : table.m_TableRows) {
        stream << "    " << tableRow << std::endl;
    }
    std::cout << "}" << std::endl;

    return stream;
}

bool isEqual(const TableRow& tableRow, const TableKey& tableKey) {
    return (
        (compare(tableRow.getTableKeyStr(), tableKey.str) == 0) &&
        (tableRow.getTableKeyNum() == tableKey.num)
    );
}

int compare(const std::string& string1, const std::string& string2) {
    return string1.compare(string2);

    // const char* const str1 = string1.c_str();
    // const char* const str2 = string2.c_str();
    // for (unsigned int i = 0; str1[i] == str2[i] && str1[i] != '\0'; i++);
    // return (str1[i] - str2[i]);
}

int compare(const TableKey& tableKey1, const TableKey& tableKey2) {
    const int strComparisonResult = compare(tableKey1.str, tableKey2.str);
    if (strComparisonResult != 0) {
        return strComparisonResult;
    } else {
        return (tableKey1.num - tableKey2.num);
    }
}

int compare(const TableRow& tableRow, const TableKey& tableKey) {
    return compare(tableRow.getTableKey(), tableKey);
}

int compareSimilarity(const std::string& str1, const std::string& str2) {
    unsigned short amount = 0;
    unsigned short i1 = 0;
    unsigned short i2 = 0;

    for (;; amount++) {
        if (i1 >= str1.size() || i2 >= str2.size()) {
            break;
        }

        const bool isWideChar1 = (unsigned short)str1[i1] > 1000;
        const bool isWideChar2 = (unsigned short)str2[i2] > 1000;
        std::string l1 = str1.substr(i1, isWideChar1 ? 2 : 1);
        std::string l2 = str2.substr(i2, isWideChar2 ? 2 : 1);

        if (areSimilar(l1, l2)) {
            i1 += isWideChar1 ? 2 : 1;
            i2 += isWideChar2 ? 2 : 1;
        } else {
            break;
        }
    }

    return amount;
}

int compareSimilarity(const TableKey& tk1, const TableKey& tk2) {
    return compareSimilarity(tk1.str, tk2.str);
}


inline const std::vector< LetterPair >& getSimilarLetters() {
    static const std::vector< LetterPair > similarLetters = {
        LetterPair("e", "е"),
        LetterPair("E", "Е"),
        LetterPair("T", "Т"),
        LetterPair("y", "у"),
        LetterPair("Y", "У"),
        LetterPair("o", "о"),
        LetterPair("O", "О"),
        LetterPair("p", "р"),
        LetterPair("P", "Р"),
        LetterPair("a", "а"),
        LetterPair("A", "А"),
        LetterPair("H", "Н"),
        LetterPair("k", "к"),
        LetterPair("K", "К"),
        LetterPair("x", "х"),
        LetterPair("X", "Х"),
        LetterPair("c", "с"),
        LetterPair("C", "С"),
        LetterPair("B", "В"),
        LetterPair("m", "м"),
        LetterPair("M", "М"),
    };

    return similarLetters;
}

bool areSimilar(const std::string& a, const std::string& b) {
    if (a.compare(b) == 0) {
        return true;
    }

    const std::vector< LetterPair > similarLetters = getSimilarLetters();
    for (const LetterPair& letterPair : similarLetters) {
        if ((letterPair.first.compare(a) == 0 &&
                letterPair.second.compare(b) == 0) ||
            (letterPair.first.compare(b) == 0 &&
                letterPair.second.compare(a) == 0)) {
            return true;
        }
    }

    return false;
}

bool isAmongSimilarLetters(const std::string& l) {
    for (const LetterPair& letterPair : getSimilarLetters()) {
        if (letterPair.first.compare(l) == 0 ||
                letterPair.second.compare(l) == 0) {
            return true;
        }
    }

    return false;
}
