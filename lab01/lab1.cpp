/*
 * System Programming
 * "Modular programming in basic program design strategy and its usage in
 * programs for table creation"
 *
 * Author: Igor Boyarshin, #5207, IO-52, FIOT
 * Date: 24.09.17
 */


#include "Table.h"
#include "TableRow.h"
#include "TableKey.h"
#include <iostream>
#include <string>


int main() {
    std::cout << "// ------------- Program Start ---------------- //"
              << std::endl;

    Table table;

    table.insertDirect(TableRow("one", 1, 1.0), 0);
    table.insertDirect(TableRow("two", 2, 2.0), 1);
    table.insertDirect(TableRow("three", 3, 3.0), 2);
    table.updateDirect(TableRow("four", 4, 4.0), 1);

    std::cout << table;

    table.insertLinear(TableRow("abc", 6, 5.0));
    table.deleteLinear(TableRow("three", 3, 3.0));
    table.insertLinear(TableRow("abc", 4, 7.0));
    std::cout << "Field: "
        << table.selectLinear(TableKey("abc", 4))->getField()
        << std::endl;

    table.sort();
    std::cout << table;

    table.insertBinary(TableRow("one", 0, 80.0));
    table.deleteBinary(TableRow("one", 0, 80.0));
    table.deleteBinary(TableRow("two", 2, 2.0));
    table.updateBinary(TableKey("abc", 4), TableRow("abc", 3, 7));
    std::cout << "Field: "
        << table.selectBinary(TableKey("abc", 6))->getField()
        << std::endl;

    std::cout << table;

    table.packLinear();

    std::cout << table;

    table.insertLinear(TableRow("AВс", 3, 3.0));
    table.insertLinear(TableRow("ABft", 3, 4.0));
    table.insertLinear(TableRow("aa", 3, 5.0));

    std::cout << table;

    std::cout << "Field of best fit: "
        << table.selectBestFit(TableKey("АВcde", 4))->getField()
        << std::endl;


    return 0;
}
