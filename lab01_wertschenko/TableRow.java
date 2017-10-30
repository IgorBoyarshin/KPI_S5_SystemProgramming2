class TableRow {
    private final RowKey rowKey;
    private double functionalField;
    private boolean hasBeenDeleted;

    public TableRow() {
        this.rowKey = new RowKey("", 0);
        this.functionalField = 0.0;
        this.hasBeenDeleted = false;
    }

    public TableRow(RowKey rowKey, double functionalField) {
        this.rowKey = rowKey;
        this.functionalField = functionalField;
        this.hasBeenDeleted = false;
    }

    public TableRow(String keyString, int keyNumber, double functionalField) {
        this.rowKey = new RowKey(keyString, keyNumber);
        this.functionalField = functionalField;
        this.hasBeenDeleted = false;
    }

    // --------------------------------------------------------------

    public boolean isEmpty() {
        return rowKey.string.length() == 0;
    }

    public void markAsDeleted() {
        hasBeenDeleted = true;
    }

    public boolean isDeleted() {
        return hasBeenDeleted;
    }

    public RowKey getRowKey() {
        return rowKey;
    }

    public String getRowKeyString() {
        return rowKey.string;
    }

    public int getRowKeyNumber() {
        return rowKey.number;
    }

    public double getFunctionalField() {
        return functionalField;
    }

    @Override
    public String toString() {
        return "  { string: " + rowKey.string
            + " || number: " + rowKey.number
            + " || functional field: " + functionalField
            + " }";
    }
}
