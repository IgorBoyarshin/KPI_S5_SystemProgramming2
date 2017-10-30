import java.util.ArrayList;

class Table {
    private ArrayList<TableRow> rows;
    private char tableLength;

    public Table() {
        this.rows = new ArrayList<>();
        this.tableLength = 0;
    }

    public Table(ArrayList<TableRow> rows) {
        this.rows = rows;
        this.tableLength = (char) rows.size();
    }

    // --------------------------------------------------------------

    public boolean isPresent(RowKey rowKey) {
        for (TableRow row : rows) {
            if (isEqual(row, rowKey)) {
                return true;
            }
        }

        return false;
    }

    public boolean isPresent(String keyString, int keyNumber) {
        return isPresent(new RowKey(keyString, keyNumber));
    }

    // --------------------------------------------------------------

    public TableRow selectDirect(char index) {
        return rows.get(index);
    }

    public boolean insertDirect(TableRow newRow, char index) {
        if (index > tableLength) {
            return false;
        }

        while (index < tableLength) {
            rows.set(index++, new TableRow());
        }
        rows.add(newRow);
        tableLength = ++index;

        return true;
    }

    public boolean deleteDirect(char index) {
        if (index < tableLength) {
            rows.get(index).markAsDeleted();
            return true;
        }

        return false;
    }

    public void updateDirect(TableRow newRow, char index) {
        if (index >= tableLength) {
            return;
        }

        rows.set(index, newRow);
    }

    // --------------------------------------------------------------

    public TableRow selectLinear(RowKey key) {
        for (TableRow row : rows) {
            if (isEqual(row, key)) {
                return row;
            }
        }

        return null;
    }

    public void insertLinear(TableRow row) {
        char index;
        for (index = 0; index < tableLength && !row.isEmpty(); index++);

        if (index == tableLength) {
            tableLength++;
            rows.add(row);
        } else {
            rows.set(index, row);
        }
    }

    public void deleteLinear(TableRow row) {
        TableRow rowToDelete = selectLinear(row.getRowKey());
        if (rowToDelete != null) {
            rowToDelete.markAsDeleted();
        }
    }

    public void updateLinear(RowKey key, TableRow newRow) {
        TableRow rowToUpdate = selectLinear(key);
        if (rowToUpdate != null) {
            rows.set(rows.indexOf(rowToUpdate), newRow);
        }
    }

    public void pack() {
        for (char i = 0; i < tableLength; i++) {
            if (rows.get(i).isDeleted()) {
                rows.remove(i);
                i--;
            }
        }

        tableLength = (char) rows.size();
    }

    // --------------------------------------------------------------

    public TableRow selectBinary(RowKey key) {
        sort();

        char l = 0;
        char r = (char) (tableLength - 1);

        while (r >= l) {
            char m = (char) ((l + r) / 2);

            TableRow row = rows.get(m);
            int comparisonResult = compare(row, key);
            if (comparisonResult == 0) {
                return row;
            } else if (comparisonResult > 0) {
                if (m < 1) {
                    break;
                }
                r = (char) (m - 1);
            } else {
                l = (char) (m + 1);
            }
        }

        return null;
    }

    public void insertBinary(TableRow row) {
        sort();

        char l = 0;
        char r = (char) (tableLength - 1);
        char m = 0;
        int comparisonResult = 0;
        final RowKey key = row.getRowKey();

        while (r >= l) {
            m = (char) ((l + r) / 2);

            comparisonResult = compare(rows.get(m), key);
            if (comparisonResult == 0) {
                rows.set(m, row);
                return;
            } else if (comparisonResult > 0) {
                if (m < 1) {
                    break;
                }
                r = (char) (m - 1);
            } else {
                l = (char) (m + 1);
            }
        }

        // no match => insert
        rows.add(m + (comparisonResult >= 0 ? 0 : 1), row);
    }

    public void deleteBinary(TableRow row) {
        TableRow foundRow = selectBinary(row.getRowKey());
        if (foundRow != null) {
            foundRow.markAsDeleted();
        }
    }

    public void updateBinary(RowKey key, TableRow row) {
        TableRow foundRow = selectBinary(key);
        if (foundRow != null) {
            rows.set(rows.indexOf(foundRow), row);
        }
    }

    public void sort() {
        rows.sort((a, b) -> compare(a.getRowKey(), b.getRowKey()));
    }

    // --------------------------------------------------------------

    public ArrayList<TableRow> getAllRowsThatFitBest(RowKey key) {
        ArrayList<TableRow> result = null;
        int currentBestFit = -1;

        for (TableRow row : rows) {
            final int rank = evaluateFitness(row.getRowKey(), key);
            if (rank > currentBestFit) {
                result = new ArrayList<>();
                result.add(row);
                currentBestFit = rank;
            } else if (rank == currentBestFit) {
                result.add(row);
            }
        }

        return result;
    }

    // --------------------------------------------------------------

    public static boolean isEqual(TableRow row, RowKey key) {
        return (compare(row.getRowKeyString(), key.string) == 0)
            && (row.getRowKeyNumber() == key.number);
    }

    public static int compare(String string1, String string2) {
        return string1.compareTo(string2);
    }

    public static int compare(RowKey key1, RowKey key2) {
        final int resultOfComparison = compare(key1.string, key2.string);
        if (resultOfComparison != 0) {
            return resultOfComparison;
        } else {
            return key1.number - key2.number;
        }
    }

    public static int compare(TableRow row, RowKey key) {
        return compare(row.getRowKey(), key);
    }

    // Calculates the amount of pairs (charFromString1; charFromStrig2),
    // where chars are equal. Each char in each string may be used only in one pair.
    public static int evaluateFitness(String string1, String string2) {
        int counter = 0;
        for (char c : string1.toCharArray()) {
            if (string2.contains(c + "")) {
                counter++;
                string2 = string2.replaceFirst(c + "", ""); // removes this char
            }
        }

        return counter;
    }

    public static int evaluateFitness(RowKey key1, RowKey key2) {
        return evaluateFitness(key1.string, key2.string);
    }

    // --------------------------------------------------------------

    @Override
    public String toString() {
        String result = "Table:\n";
        for (TableRow row : rows) {
            result += row + "\n";
        }

        return result;
    }
}
