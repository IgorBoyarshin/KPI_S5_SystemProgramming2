import java.util.ArrayList;

class Table {

    private ArrayList<Row> rows;
    private char length;

    public Table() {
        this.rows = new ArrayList<>();
        this.length = 0;
    }

    public Table(ArrayList<Row> rows) {
        this.rows = rows;
        this.length = (char) rows.size();
    }


    public boolean isPresent(Key key) {
        for (Row row : rows) {
            if (isEqual(row, key)) {
                return true;
            }
        }

        return false;
    }

    public boolean isPresent(String keyString, short keyNum) {
        return isPresent(new Key(keyString, keyNum));
    }


    public Row directSelect(char index) {
        return rows.get(index);
    }

    public boolean directInsert(Row row, char index) {
        if (index > length) {
            return false;
        }

        while (index < length) {
            rows.set(index++, new Row());
        }
        rows.add(row);
        length = ++index;

        return true;
    }

    public boolean directDelete(char index) {
        if (index < length) {
            rows.get(index).markAsDeleted();
            return true;
        }

        return false;
    }

    public void directUpdate(Row newRow, char index) {
        if (index >= length) {
            return;
        }

        rows.set(index, newRow);
    }


    public Row linearSelect(Key key) {
        for (Row row : rows) {
            if (isEqual(row, key)) {
                return row;
            }
        }

        return null;
    }

    public void linearInsert(Row row) {
        char index;
        for (index = 0; index < length && !row.isEmpty(); index++);

        // reached end, no empty elements found
        if (index == length) {
            length++;
            rows.add(row);
        } else {
            rows.set(index, row);
        }
    }

    public void linearDelete(Row row) {
        Row rowToDelete = linearSelect(row.getKey());
        if (rowToDelete != null) {
            rowToDelete.markAsDeleted();
        }
    }

    public void linearUpdate(Key key, Row row) {
        Row rowToUpdate = linearSelect(key);
        if (rowToUpdate != null) {
            rows.set(rows.indexOf(rowToUpdate), row);
        }
    }

    public void linearPack() {
        for (char i = 0; i < length; i++) {
            if (rows.get(i).isDeleted()) {
                rows.remove(i);
                i--;
            }
        }

        length = (char) rows.size();
    }

    public void sort() {
        rows.sort((a, b) -> compare(a.getKey(), b.getKey()));
    }


    public Row binarySelect(Key key) {
        sort();

        char l = 0;
        char r = (char) (length - 1);

        while (r >= l) {
            char m = (char) ((l + r) / 2);

            Row row = rows.get(m);
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

    public void binaryInsert(Row row) {
        Key key = row.getKey();
        sort();

        char l = 0;
        char r = (char) (length - 1);
        char m = 0;
        int comparisonResult = 0;
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

    public void binaryDelete(Row row) {
        Row foundRow = binarySelect(row.getKey());
        if (foundRow != null) {
            foundRow.markAsDeleted();
        }
    }

    public void binaryUpdate(Key key, Row row) {
        Row foundRow = binarySelect(key);
        if (foundRow != null) {
            rows.set(rows.indexOf(foundRow), row);
        }
    }

    public ArrayList<Row> selectRowsWithBestMatch(Key key) {
        ArrayList<Row> result = null;
        int currentMaxRank = -1;

        for (Row row : rows) {
            final int rank = rankSimilarity(row.getKey(), key);
            if (rank > currentMaxRank) {
                result = new ArrayList<>();
                result.add(row);
                currentMaxRank = rank;
            } else if (rank == currentMaxRank) {
                result.add(row);
            }
        }

        return result;
    }


    @Override
    public String toString() {
        String result = "Table:\n";
        for (Row row : rows) {
            result += "  " + row + "\n";
        }

        return result;
    }

    public static boolean isEqual(Row row, Key key) {
        return (compare(row.getKeyString(), key.str) == 0) && (row.getKeyNum() == key.num);
    }

    public static int compare(String string1, String string2) {
        return string1.compareTo(string2);
    }

    public static int compare(Key key1, Key key2) {
        final int resultOfComparison = compare(key1.str, key2.str);
        if (resultOfComparison != 0) {
            return resultOfComparison;
        } else {
            return key1.num - key2.num;
        }
    }

    public static int compare(Row row, Key key) {
        return compare(row.getKey(), key);
    }

    public static int rankSimilarity(String string1, String string2) {
        string1 = new String(string1.toLowerCase());
        string2 = new String(string2.toLowerCase());

        int counter = 0;
        for (char c : string1.toCharArray()) {
            if (string2.contains(c + "")) {
                counter++;
                string2 = string2.replaceFirst(c + "", "");
            }
        }

        // System.out.println(">> " + string1 + " | " + string2 + " | " + counter);
        return counter;
    }

    public static int rankSimilarity(Key key1, Key key2) {
        return rankSimilarity(key1.str, key2.str);
    }
}
