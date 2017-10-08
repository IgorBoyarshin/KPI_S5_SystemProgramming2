class Row {
    private final Key key;
    private Field field;
    private boolean deleted;

    public Row() {
        this.key = new Key("", (short)0);
        this.field = Field.FieldValue1;
        this.deleted = false;
    }

    public Row(Key key, Field field) {
        this.key = key;
        this.field = field;
        this.deleted = false;
    }

    public Row(String keyString, short keyNum, Field field) {
        this.key = new Key(keyString, keyNum);
        this.field = field;
        this.deleted = false;
    }


    public boolean isEmpty() {
        return key.str.length() == 0;
    }

    public void markAsDeleted() {
        deleted = true;
    }

    public boolean isDeleted() {
        return deleted;
    }

    public Key getKey() {
        return key;
    }

    public String getKeyString() {
        return key.str;
    }

    public short getKeyNum() {
        return key.num;
    }

    public Field getField() {
        return field;
    }

    @Override
    public String toString() {
        return "Row (str: " + key.str
            + "; num: " + key.num
            + "; field: " + field
            + "; )";
    }
}
